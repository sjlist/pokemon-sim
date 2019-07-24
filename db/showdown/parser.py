#!/usr/bin/env python
# coding: utf-8


import hjson # for dealing with improper JSON
import json
import requests
import os


names = ['pokedex', 'moves', 'abilities']
saveJSON = False


import pymongo

# connect to MongoDB instance
mongoclient = pymongo.MongoClient("mongodb://localhost:27017/")

# create db (not created until it's non-empty)
db = mongoclient["showdown"]


# make the json files corresponding to the names above
for file in names:
    # where to find file
    url = 'https://play.pokemonshowdown.com/data/' + file + '.js'
    
    # get the javascript code for the table
    response = requests.get(url)
    
    # decode content bytes object to string
    response = response.content.decode('utf-8')
    
    # crop string to almost json format
    # remove variable name and semicolon
    response = response[response.find('{'):-1]
    
    # decode as json
    response = hjson.loads(response)
    
    # save json
    if saveJSON:
        try:
            os.mkdir('./json')
        except:
            print('./json already exists')
        hjson.dumpJSON(response, open('./json/'+file+'.json', 'w'))
        
    # convert to dictionary
    response = json.loads(hjson.dumpsJSON(response))
    
    # create collection for the file downloaded
    collection = db[file]
    
    # save each element in response to collection
    for key in response.keys():
        collection.insert_one(response[key])



