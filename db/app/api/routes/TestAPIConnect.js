const express = require('express');
const router = express.Router();

router.get('/', (req, res) => {
  console.log('api test get recieved');
  res.send({ connection: 'Connected to api' }); 
});

module.exports = router;
