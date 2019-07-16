const mongoose = require('mongoose');

const MoveModel = new mongoose.Schema({
	damage_type: String,
	type: String, 
	priority: Number,
	pp: Number,
	acc: Number,
	targeting: String,
	num_targets: Number,
	damage_info: { power: Number, crit_change: Number },
	effects: [{effect: String, status: String, change: Number}], 
	contact: { type: Boolean }
})

// export User model for use in other files.
module.exports = mongoose.model('move', MoveModel, 'moves');
