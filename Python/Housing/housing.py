import numpy as np
import pandas as pd

import os
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import norm
from scipy import stats


from sklearn_pandas import DataFrameMapper
from sklearn_pandas import CategoricalImputer
#from sklearn.preprocessing import Imputer
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import FunctionTransformer

from sklearn.base import TransformerMixin
from sklearn.compose import ColumnTransformer


from sklearn.pipeline import Pipeline
from sklearn.pipeline import FeatureUnion

from sklearn.tree import DecisionTreeRegressor
from sklearn.linear_model import Ridge
from sklearn.linear_model import Lasso
from sklearn.linear_model import ElasticNet
from xgboost import XGBRegressor
from sklearn.model_selection import GridSearchCV


'''

class Categorical_Imputer(TransformerMixin):
    def __init__(self):
        """Impute missing values.

        Columns of dtype object are imputed with the most frequent value 
        in column.

        Columns of other types are imputed with mean of column.

        """
        #self.strategy = strategy


    def fit_transform(self,X,y=None):
        col_all = X.columns
        for col in col_all:
            #print(col,' ',X[col].value_counts().index[0])
            X[col].fillna(X[col].value_counts().index[0],inplace=True)
            #X.info()
        return X

'''

class Categorical_Imputer(TransformerMixin):
    def __init__(self,decisiondict):
        """Impute missing values.

        Columns of dtype object are imputed with the most frequent value 
        in column.

        Columns of other types are imputed with mean of column.

        """
        self.decisiondict = decisiondict


    def fit_transform(self,X,y=None):
        col_all = X.columns
        dict_keys = list(self.decisiondict.keys())
        #print(dict_keys)
        for col in col_all:

            if col in dict_keys:
                decision = self.decisiondict[col]
                print(col,decision)
                if "Replacewith" in decision:
                    _,var = decision.split("_")
                    var = str(var)
                    print('Repalcing  {} with {}'.format(col,var))
                    print(col,type(var))
                    X[col].fillna(var,inplace=True)    
            else:
                X[col].fillna(X[col].value_counts().index[0],inplace=True)
            #X.info()
        return X

 
 
 
def model_fit(train_features, train_actuals):
        """
        fits the list of models to the training data, thereby obtaining in each 
        case an evaluation score after GridSearchCV cross-validation
        """
        for name in models.keys():
            est = models[name]
            est_params = params[name]
            gscv = GridSearchCV(estimator=est, param_grid=est_params, cv=5,
                               scoring='neg_mean_absolute_error', return_train_score=True)
            gscv.fit(train_actuals, train_features)
            cvres = gscv.cv_results_
            print(cvres)
            print("best parameters are: {}".format(gscv.best_estimator_))
            for mean_score,par in zip(cvres["mean_test_score"],cvres["params"]):
                print(-mean_score, par)
            
            
            


if __name__ == "__main__":
    train_df = pd.read_csv('train.csv')
    test_df  = pd.read_csv('test.csv')
    
    y_train = train_df['SalePrice']
    train_df = train_df.drop(['Id','SalePrice'],axis=1)
    test_df = test_df.drop('Id',axis=1)
    
    test_length = len(test_df)
    train_length = len(train_df)

    #concatenate and get the numeric/category columns
    all_sample = pd.concat([train_df,test_df])
    
    all_sample['MSSubClass'] = pd.Categorical(all_sample.MSSubClass)
    all_sample['MSSubClass'] = all_sample['MSSubClass'].astype(str)

    num_col = list(all_sample._get_numeric_data().columns)
    cat_col = list(set(all_sample.columns) - set(num_col))

    #preparing function to select all numeric and categorical variables
    select_numeric = FunctionTransformer(lambda x : x[num_col],validate=False)
    select_cate = FunctionTransformer(lambda x : x[cat_col],validate=False)

    decision_dict = {'MSZoning':'Replacewith_RL',
                    'Alley':'Replacewith_NA',
                    'Utilities':'Replacewith_AllPub',
                    'Exterior1st':'Replacewith_VinylSd',
                    'Exterior2nd':'Replacewith_VinylSd',
                    'MasVnrType':'Replacewith_None',
                    'BsmtQual':'Replacewith_NA',
                    'BsmtCond':'Replacewith_NA',
                    'BsmtExposure':'Replacewith_NA',
                    'BsmtFinType1':'Replacewith_NA',
                    'BsmtFinType2':'Replacewith_NA',
                    'KitchenQual':'Replacewith_TA',
                    'Functional':'Replacewith_Typ',
                    'FireplaceQu':'Replacewith_NA',
                    'GarageType':'Replacewith_NA',
                    'GarageYrBlt':'ROWReplace_YearBuilt',
                    'GarageFinish':'Replacewith_NA',
                    'GarageQual':'Replacewith_TA',
                    'GarageCond':'Replacewith_TA',
                    'PoolQC':'Replacewith_NA',
                    'Fence':'Replacewith_NA',
                    'MiscFeature':'Replacewith_NA',
                    'SaleType':'Replacewith_WD',
                    'MSSubClass':'Replacewith_20'}
    cate_imputer = Categorical_Imputer(decision_dict)
    cate_imp = FunctionTransformer(cate_imputer.fit_transform,validate=False)
    
    #convert categorical to numeric
    get_dummies = FunctionTransformer(lambda x: pd.get_dummies(x, prefix_sep='_', drop_first=True), 
                                  validate=False)


    #simple mode for the imputer. A class can be written in the future
    num_imputer = SimpleImputer(strategy="most_frequent")
    num_standard = StandardScaler()
    num_pipeline = Pipeline(steps=
                        [('select_num',select_numeric),
                         ('imp_num',num_imputer),
                        ('standadize',num_standard)])
    
    num_pipeline.fit_transform(all_sample)


    cate_pipeline = Pipeline(steps=[
                                ('select_cate',select_cate),
                                ('imp_cate',cate_imp),
                                ('get_dummy',get_dummies)])
    cate_pipeline.fit_transform(all_sample)
    # merge numerical and categorical variables 
    union = ColumnTransformer([
        ('numerical',num_pipeline,num_col),
        ('categorical',cate_pipeline,cat_col)])
    
    union_result = union.fit_transform(all_sample)
    print('union shape: ',union_result.shape)

    X_train = union_result[:train_length]
    X_test  = union_result[train_length:]

    models = {
    'RidgeRegression':Ridge(),
    'LassoRegression':Lasso()}#,
    #'ElasticNetRegression':ElasticNet(),
    #'XBoostRegression':XGBRegressor()}

    params = {
        'RidgeRegression':{
            "alpha":[5,10,20],
            "normalize":[True,False]},
        'LassoRegression':{
            "alpha":[5,10,20],
            "normalize":[True,False]}}
      #  'ElasticNetRegression':{
      #      "alpha":[5,10,20],
      #      "normalize":[True,False],
      #      "l1_ratio":[0.5]},
      #  'XBoostRegression':{
      #      "learning_rate":[0.05,0.1],
      #      "n_estimators":[10,50,100]}
    #}
    #model_fit(y_train,X_train)

    from Extra_Package.EstimatorSelector import EstimatorSelectionHelper
    helper1 = EstimatorSelectionHelper(models,params)
    helper1.fit(X_train,y_train)
    helper1.score_summary()
