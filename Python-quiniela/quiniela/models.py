from sklearn.tree import DecisionTreeClassifier

import pickle


class QuinielaModel:
    
    def __init__(self):
        self.features = ['division','matchday','season_encoded','team_encoded',
                    'away_team_encoded','GD_home','Pts_difference','home_rank']
        self.target = ['result_home_encoded']
        self.classifier = DecisionTreeClassifier(max_depth=7) 

        
    def train(self, train_data):
        X_train = train_data[self.features]
        y_train = train_data[self.target]
        model = self.classifier.fit(X_train, y_train)
        pass

    def predict(self, predict_data):
        
        return [value for values in range(len(predict_data))]

    @classmethod
    def load(cls, filename):
        """ Load model from file """
        with open(filename, "rb") as f:
            model = pickle.load(f)
            assert type(model) == cls
        return model

    def save(self, filename):
        """ Save a model in a file """
        with open(filename, "wb") as f:
            pickle.dump(self, f)
