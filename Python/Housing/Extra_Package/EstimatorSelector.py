import pandas as pd 
import numpy as np 

from sklearn.model_selection import GridSearchCV

class EstimatorSelectionHelper:
    
    def __init__(self,models,params):
        if not set(models.keys()).issubset(set(params.keys())):
            missing_par = list(set(models.keys()) - set(params.keys()))
            raise ValueError("parameter missing %s" % missing_par)
        self.models = models
        self.params = params
        self.keys = models.keys()
        self.grid_searchs = {}

    def fit(self,X,y,n_jobs=3,verbose=1,scoring='neg_mean_absolute_error',return_train_score=True):
        for key in self.keys:
            print("Running GridSearch for %s" % key)
            model = self.models[key]
            params = self.params[key]
            gs = GridSearchCV(estimator=model,param_grid=params,
                    scoring=scoring,return_train_score=True,cv=5)
            gs.fit(X,y)
            self.grid_searchs[key] = gs
    def score_summary(self):
        def row(key,test_scores,train_scores,params):
            d = {
                'estimator':key,
                'min_test_score':min(test_scores),
                'max_test_score':max(test_scores),
                'std_test_score':np.std(test_scores),
                'min_train_score':min(train_scores),
                'max_train_score':max(train_scores),
                'std_train_score':np.std(train_scores),
            }
            return pd.Series({**params,**d})
            #return pd.Series(d)
        rows = []

        for k in self.grid_searchs:
            print(k)
            params = self.grid_searchs[k].cv_results_['params']
            test_scores = []
            train_scores = []
            print('check 2', self.grid_searchs[k].cv)
            for i in range(self.grid_searchs[k].cv):
                key_test = "split{}_test_score".format(i)
                key_train = "split{}_train_score".format(i)
                r_test = self.grid_searchs[k].cv_results_[key_test]*-1
                r_train = self.grid_searchs[k].cv_results_[key_train]*-1
                test_scores.append(r_test.reshape(len(params),1))
                train_scores.append(r_train.reshape(len(params),1))

            all_test_scores = np.hstack(test_scores)
            all_train_scores = np.hstack(train_scores)
            for p,s_test,s_train in zip(params,all_test_scores,all_train_scores):
                rows.append((row(k,s_test,s_train,p)))
            df = pd.concat(rows,axis=1).T
            columns = ['estimator','min_test_score','max_test_score','std_test_score','min_train_score','max_train_score','std_train_score']
            columns = columns + [c for c in df.columns if c not in columns]
            print(df)
        return df

                
