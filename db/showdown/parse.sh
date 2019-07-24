#!/bin/bash

# launch mongo via docker
docker run -p 27017:27017 -d --rm --name pokemon_mongo mongo

# give mongo time to start up
sleep 5 

# set saveJSON and list of files/collections in the file
python3 parser.py

# dump db
mongodump --db showdown --out ../db

# stop mongo instance (removes automatically b/c the --rm flag)
docker stop pokemon_mongo
