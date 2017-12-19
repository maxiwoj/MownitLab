from flask import Flask, render_template, request
import numpy as np
import pandas as pd
import re
from scipy.sparse import csr_matrix
from tqdm import tqdm
import scipy
import itertools
from math import log
from sklearn.feature_extraction.text import CountVectorizer, TfidfTransformer, TfidfVectorizer
from sklearn.externals import joblib
import heapq

def parse_words(tokens, ps, wnl):
    for t in tokens:
        t = t.lower()
        t = wnl.lemmatize(t)
        t = re.sub('[^a-z]', '', t)
        t = ps.stem(t)
        yield t

class CustomVectorizer(TfidfVectorizer):
    def __init__(self, stop_words='english'):
        super(CustomVectorizer, self).__init__(stop_words=stop_words)
#         ps = PorterStemmer()
        self.ps = snowball.EnglishStemmer()
        self.wnl = WordNetLemmatizer()
    def build_tokenizer(self):
        tokenize = super(CustomVectorizer, self).build_tokenizer()
        return lambda doc: list(parse_words(tokenize(doc), self.ps, self.wnl))

def load_sparse_csr(filename):
    loader = np.load(filename)
    return csr_matrix((  loader['data'], loader['indices'], loader['indptr']),
                         shape = loader['shape'])

vectorizer = joblib.load('vectorizer.pkl')
bag_of_words = load_sparse_csr('bag_of_words.npz').toarray()
bag_of_words_svd = np.load('bag_of_words_svd.npy')
articles = pd.read_csv('buzzfeed.csv').head(15000)

app = Flask(__name__)

@app.route("/")
def main():
    return render_template('index.html')



def count_non_zero(bag_of_words):
    if type(bag_of_words) == scipy.sparse.csr.csr_matrix:
        return bag_of_words.count_nonzero()
    else:
        return np.count_nonzero(bag_of_words)

def get_best_result(query, bag_of_words, k=10):
    query_vec = vectorizer.transform([query])[0]
    query_length = query_vec.count_nonzero()
    number_of_docs = articles.shape[0]
    matches = dict()
    for i in tqdm(range(number_of_docs)):
        matches[i] = 0
        for word_index in query_vec.indices:
            matches[i] += bag_of_words[i, word_index]
        matches[i] /= (query_length * count_non_zero(bag_of_words[i]))
#         query_vec.multiply(bag_of_words[i]).sum()/(query_length * count_non_zero(bag_of_words[i]))
    return list(dict(heapq.nlargest(k, matches.items(), key=lambda x: x[1])))

def print_urls(best_results):
    best_results = list(map(lambda x: texts['url'][x], best_results))
    for i, result in enumerate(best_results):
        print(i, ': ', result)



@app.route("/basic", methods=['GET', 'POST'])
def basicSearch():
	best_results=[]
	if request.method == 'POST':
		best_results = list(map(lambda x: articles['url'][x],get_best_result(request.form['query'], bag_of_words)))
		
	return render_template('basicSearch.html', best_matches=best_results)

@app.route("/svd", methods=['GET', 'POST'])
def svdSearch():
	best_results=[]
	if request.method == 'POST':
		best_results = list(map(lambda x: articles['url'][x],get_best_result(request.form['query'], bag_of_words_svd)))
		
	return render_template('svdSearch.html', best_matches=best_results)

# @app.route("/basic/search/", method='POST')
# def search():
	# query = request.


if __name__ == "__main__":
    app.run()




