#!/usr/bin/python

# Imports:
import sys, requests, json, sqlite3, os

# Constants:
URL = 'https://opentdb.com/api.php?type=multiple&amount='
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "TriviaDB.db")
INSERT_QUESTION = 'INSERT INTO QUESTIONS VALUES("{}", "{}", "{}", "{}", "{}")'

# main function:
def main():
    # Getting the questions:
    data = requests.get(URL + sys.argv[1]).json()

    # Connecting to the database:
    dbCon = sqlite3.connect(DB_PATH, timeout=3)
    dbCur = dbCon.cursor()

    # Adding the questions to the database:
    for question in data['results']:
        dbCur.execute(INSERT_QUESTION.format(question['question'], \
            question['incorrect_answers'][0], question['incorrect_answers'][1], \
            question['incorrect_answers'][2], question['correct_answer']))
        dbCon.commit()
    
    # Closing the connection to the database:
    dbCon.close()

if __name__ == '__main__':
    main()