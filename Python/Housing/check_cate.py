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
def get_nanbycol (data):
    all_nan = data.isnull().sum(axis = 0)
    nan_df = pd.DataFrame(all_nan,columns=['nans'])
    abovezero = nan_df[nan_df['nans']>0]
    print(abovezero)
    return abovezero

if __name__ == "__main__":

    train_df = pd.read_csv('train.csv')
    test_df  = pd.read_csv('test.csv')
    train_label_df = train_df['SalePrice']
    train_df = train_df.drop(['Id','SalePrice'],axis=1)
    test_df = test_df.drop('Id',axis=1)
    all_sample = pd.concat([train_df,test_df])

    all_sample['MSSubClass'] = pd.Categorical(all_sample.MSSubClass)
    all_sample['MSSubClass'] = all_sample['MSSubClass'].astype(str)

    num_col = list(all_sample._get_numeric_data().columns)
    cat_col = list(set(all_sample.columns) - set(num_col))
    
    get_nanbycol(all_sample[cat_col])
    print(all_sample['PoolQC'].value_counts())
    print(all_sample['PoolQC'])
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
    #cate_imputer.fit_transform(all_sample[cat_col])
    all_sample[cat_col] = cate_imputer.fit_transform(all_sample[cat_col])
    print(all_sample['PoolQC'])
    
    #print(all_sample['BsmtExposure'].ix[index_of_nan[0][0]])



 
 