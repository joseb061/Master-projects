import pickle


class QuinielaModel:
    
    def __init__(self,model_name):
        self.features = ['division','matchday','season_encoded','team_encoded',
                    'Away_team_encoded','GD_home','Pts_difference','home_rank']
        self.target = ['result_home_encoded']
        self.classifier = model_name 
        
    def train(self, train_data):
        X_train = train_data[self.features]
        y_train = train_data[self.target]
        self.classifier.fit(X_train, y_train) 
        

    def predict(self,predict_data): 
        X_test = predict_data[self.features]
        prediction = self.classifier.predict(X_test)      
        return prediction

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

