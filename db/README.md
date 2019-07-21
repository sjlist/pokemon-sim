# Pokemon Sim API

We're trying to build a MongoDB database for the Pokemon Battle Simulator. I decided to use MongoDB because it uses JSON files which are easy to read with the Boost/ptree library in c++ and are flexible for storing properties of pokemon, moves, etc. 

There are two ways we're going about building this database:
1. Make a web app to aid in manual entry and
2. Write a script to take the data from somewhere else.

In both cases, we're building a database that can be hooked into by the Battle Simulator. 


## Web App

The website is located in the app directory. I'm using React for the frontend and Express to link the frontend to the MongoDB database. The current functionality is limited to CRUD (create, remove, update, delete) of pokemon and moves. Teams have not been implemented. 

Another thing I wanted to learn more about for this project is docker-compose. For this reason, I've set up the web app so that it runs via docker-compose. Because I need to make sure the containers were properly networking, I had to change the adresses in a way that makes it unable to run outside of docker-compose. Refer to commit `f565c19ce961471270c000e096f2fc954a36d1d3` for the changes to rever to run outside of docker-compose. 


### Build

All of the `docker-compose` commands must be from the `app` directory. For simplicity, I'll assume all the commands are run in that directory. 

To build the app first install docker and docker-compose. Then run `docker-compose build`. Notice that because we're uing the mongo docker image rather than installing it ourselves, this should not need an installation of mongo. 

### Run

Simply run the app with `docker-compose up`. When you want to stop the app you can use `docker-compose down` or `Ctrl + C`. 

Once the app is running, the database will be empty. To load the existing database use the command from the app directory `mongorestore ../db`. 

**Before you close the app** make sure to dump the database to a known location using the command (from the app directory) `mongodump --out ../db/ --db pokemon`. This creates two files for each collection in the pokemon database and saves them with the correct heirarchy in the database. 

Note that if we are concerned about file size, both `mongorestore` and 	`mongodump` have a `--gzip` flag that should compress the stored database. 


## Manual/Script Database Creation

To do


## To-do

- make teams api
- auto restore db on lauch
- auto dump db on close

