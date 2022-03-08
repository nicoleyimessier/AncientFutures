import json
import nltk
import sys
from nltk.sentiment import SentimentIntensityAnalyzer

#sentiment analysis 
sia = SentimentIntensityAnalyzer()

secret = sys.argv[1]
analysis = sia.polarity_scores(secret)
print(analysis)

result = json.dumps(analysis)

file = sys.argv[2]
f = open(file, "x")
f.write(result)
f.close()
