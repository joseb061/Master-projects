import sqlite3
import pandas as pd
import settings


def load_matchday(season, division, matchday):
    # with sqlite3.connect(settings.DATABASE_PATH) as conn:
    #     data = pd.read_sql(f"""
    #         SELECT * FROM Matches
    #             WHERE season = '{season}'
    #               AND division = {division}
    #               AND matchday = {matchday}
    #     """, conn)
    data = pd.read_csv(r'provisional.csv', index_col = 0)
    data = data[(data["season"] == season) & (data["division"] == division) & (data["matchday"] == matchday)]
    if data.empty:
        raise ValueError("There is no matchday data for the values given")
    return data


def load_historical_data(seasons):
    if seasons == "all":
        data = pd.read_csv(r'provisional.csv', index_col = 0)
    else:
        data = pd.read_csv(r'provisional.csv', index_col = 0)
        data = data[data["season"] == seasons]
    if data.empty:
        raise ValueError(f"No data for seasons {seasons}")
    return data


def save_predictions(predictions):
    with sqlite3.connect(settings.DATABASE_PATH) as conn:
        predictions.to_sql(name="Predictions", con=conn, if_exists="append", index=False)
