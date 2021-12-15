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
    dataset = dataset.drop(categorical, axis=1)
    return dataset,le


def deencoding(dataset,le):
    """
    This function makes the de-encoding for the
    categorical values in .csv
    """


if __name__ == "__main__":
    categorical = ["season", "team", "Away_team", "result_home"]
    dataset = pd.read_csv(r'/home/joseba/Master/Master-projects/Python-quiniela/analysis/Definitive_ML_Data.csv', index_col = 0)
    dataset = dataset[(dataset["season"] == "1928-1929") & (dataset["division"] == 1) & (dataset["matchday"] == 1)]
    data_new = encoding(dataset, categorical)[0]
    print (data_new.head(10))
    
