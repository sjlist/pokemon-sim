# Pokemon Sim API

This API is to help add pokemon, moves, and parties to a database we can use in the simulator. 

I've been meaning to learn more about docker compose and it seems like the tool for this job, so I'll be following this tutorial to get started: https://medium.com/free-code-camp/create-a-fullstack-react-express-mongodb-app-using-docker-c3e3e21c4074. 

## Build

I will assume docker is already installed.

1. cd pokemon-sim/db/app
2. run `npm install` in both api and client directories
3. run docker pull mongo

## Run

1. run npm start in both api and client directories
2. run docker run -it -p 27017:27017 --name pokemon-mongo -v /Users/altonbarbehenn/Documents/Projects/pokemon-sim/db/app/db:/data/db -d mongo
3. go to localhost:3000

## Save

To save the database in a nice way run: `mongodump --out db/ --db pokemon`

## To-do

- make full pokemon api
- make full moves api
- make full teams api
- auto dump db when updated ? 
- Make docker images
