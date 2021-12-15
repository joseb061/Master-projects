import sqlite3
import pandas as pd
from pandas.io.formats.format import CategoricalFormatter
import settings

from quiniela import encodingg


def load_matchday(season, division, matchday):

    data = pd.read_csv(r'provisional.csv', index_col = 0)
    data = data[(data["season"] == season) & (data["division"] == division) & (data["matchday"] == matchday)]
    categorical = ["season", "team", "Away_team", "result_home"]
    data = encodingg.encoding(data, categorical)[0]
    if data.empty:
        raise ValueError("There is no matchday data for the values given")
    return data


def load_historical_data(seasons):  
    categorical = ["season", "team", "Away_team", "result_home"]
    if seasons == "all":
        data = pd.read_csv(r'provisional.csv', index_col = 0)
        data = encodingg.encoding(data, categorical)[0]
    else:
        data = pd.read_csv(r'provisional.csv', index_col = 0)
        data = data[data["season"] == seasons]
        data = encodingg.encoding(data, categorical)[0]
    if data.empty:
        raise ValueError(f"No data for seasons {seasons}")
    return data


def save_predictions(predictions):
    with sqlite3.connect(settings.DATABASE_PATH) as conn:
        predictions.to_sql(name="Predictions", con=conn, if_exists="append", index=False)
