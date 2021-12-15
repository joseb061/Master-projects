## LaQuiniela of LaLiga

Team members: 

- Jorge Vicente Puig - 1620036 
- Joseba Hernandez Bravo - 1618154

This repo contains a ML model that predicts the outcome of a matchday in LaLiga (Spanish Football League).

It also contains a ipybb called ```LaLigaDataAnalysis.ipynb``` with all the exercises done from ```DataAnalysisExercises.pdf```.

### Repository structure

```
quiniela/
  ├─── analysis/				# Jupyter Notebooks used to explore the data
  │          ...
  ├─── logs/					# Logs of the program are written
  │          ...
  ├─── models/					# The place were trained models are stored
  │          ...
  ├─── quiniela/				# Main Python package
  │          ...
  ├─── reports/					# The place to save HTML / CSV / Excel reports
  │          ...
  ├─── .gitignore
  ├─── cli.py					# Main executable. Entrypoint for CLI
  ├─── laliga.sqlite			# The database
  ├─── README.md
  ├─── requirements.txt			# List of libraries needed to run the project
  └─── settings.py				# General parameters of the program
```

### How to run it

Once you've installed dependences (```pip install -r requirements.txt```), which is only Pandas in this dummy case, you can try it yourself:

```console
foo@bar:~$ python cli.py train --training_seasons 2010:2020
Model succesfully trained and saved in ./models/my_quiniela.model
foo@bar:~$ python cli.py predict 2021-2022 1 3
Matchday 3 - LaLiga - Division 1 - Season 2021-2022
======================================================================
         RCD Mallorca          vs            Espanyol            --> predictedResult1
           Valencia            vs             Alavés             --> predictedResult2
        Celta de Vigo          vs            Athletic            --> predictedResult3
        Real Sociedad          vs            Levante             --> predictedResult4
           Elche CF            vs           Sevilla FC           --> predictedResult5
          Real Betis           vs          Real Madrid           --> predictedResult6
          Barcelona            vs             Getafe             --> predictedResult7
           Cádiz CF            vs           CA Osasuna           --> predictedResult8
        Rayo Vallecano         vs           Granada CF           --> predictedResult9
       Atlético Madrid         vs           Villarreal           --> predictedResult10
```

Here, we call ```train``` to train the model using seasons from 2010 to 2020, and then we perfom a prediction of 3rd matchday of 2021-2022 season at 1st Division using ```predict```. 

### Data

The data is provided as a csv database (Definitive_ML_Data.csv) that is inside the reports folder. This database contains the following Columns:
   * ```Away_team```,```division```,```matchday```,```result_home```,```season```,```team```,```GD_home```,```Pts_difference```,```home_rank```.

The data is derived from a a SQLite3 database that is inside the ZIP file. This database contains the following tables:

   * ```Matches```: All the matches played between seasons 1928-1929 and 2021-2022 with the date and score. Columns are ```season```,	```division```, ```matchday```, ```date```, ```time```, ```home_team```, ```away_team```, ```score```. Have in mind there is no time information for many of them and also that it contains matches still not played from current season.
   * ```Predictions```: The table for you to insert your predictions. It is initially empty. Columns are ```season```,	 ```timestamp```, ```division```, ```matchday```, ```home_team```, ```away_team```, ```prediction```, ```confidence```.

The data source is [Transfermarkt](https://www.transfermarkt.com/), and it was scraped using Python's library BeautifulSoup4.

