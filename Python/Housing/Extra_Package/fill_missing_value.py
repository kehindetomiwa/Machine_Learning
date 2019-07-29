
#from sklearn_pandas import CategoricalImputer
from sklearn.base import TransformerMixin
import numpy as np
import pandas as pd 

class Categorical_Imputer(TransformerMixin):
    def __init__(self):
        """Impute missing values.

        Columns of dtype object are imputed with the most frequent value 
        in column.

        Columns of other types are imputed with mean of column.

        """
        #self.strategy = strategy


    def fit(self,X,y=None):
        if   X.dtype == np.dtype('O'): self.fill = X.value_counts().index[0]
        else                            : self.fill = X.mean()
        return self
    def transform(self,X,y=None):
        return X.fillna(self.fill)

 