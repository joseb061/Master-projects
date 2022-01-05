import pandas as pd 

from sklearn import preprocessing

def encoding(dataset, categorical):
    """
    This function makes the encoding for the
    categorical values in .csv
    """
    le = preprocessing.LabelEncoder()
    categorical_variables = categorical
    for col in categorical_variables:
        dataset[f"{col}_encoded"] = le.fit_transform(dataset[col])
    #dataset = dataset.drop(categorical, axis=1)
    return dataset,le




    
