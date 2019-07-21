FROM node

# Create app directory
WORKDIR /usr/src/app

# Install app dependencies
COPY package*.json ./
RUN npm install --silent

# Bundle app source
COPY . .

EXPOSE 3000
CMD [ "npm", "start" ]