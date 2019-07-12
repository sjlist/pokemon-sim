# Pokemon Sim API

This API is to help add pokemon, moves, and parties to a database we can use in the simulator. 

I've been meaning to learn more about docker compose and it seems like the tool for this job, so I'll be following this tutorial to get started: https://medium.com/free-code-camp/create-a-fullstack-react-express-mongodb-app-using-docker-c3e3e21c4074. 

## Build

I will assume docker is already installed.

1. cd pokemon-sim/API
2. docker build -t pokemonapi .

## Run

1. docker run -it -p 3000:3000 pokemonapi
2. go to localhost:3000

## To-do

- Make docker image
- Make website
