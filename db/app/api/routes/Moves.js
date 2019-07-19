const mongoose = require('mongoose');
const express = require('express');
const router = express.Router();

// Load moves model
var MoveModel = require('../models/MoveModel');

// Define move api
router.get('/', (req, res) => {
	MoveModel.find({}, function(err, move) {
		if (err) throw err;
		console.log(move);
		res.send(move);
	});
});

router.post('/', (req, res) => {
	let newMove = new MoveModel(req.body);
	console.log(newMove);
	newMove.save(function (err) {
		if (err) res.send(err);
	});
	res.send(newMove);
});

router.delete('/', (req, res) => {
	console.log('delete recieved:', req.body);
	MoveModel.deleteOne({_id: req.body._id}, function (err) {
		if (err) res.send(err);
	});
	res.send(req.body);
});

router.put('/', (req, res) => {
	MoveModel.updateOne({_id: req.body._id}, req.body, function (err) {
		if (err) res.send(err);
	});
	res.send(req.body);
	console.log('recieved update:', req.body);
});

module.exports = router;
