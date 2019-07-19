const mongoose = require('mongoose');

const MoveModel = new mongoose.Schema({
	name: String,
	damage_type: String,
	type: String, 
	priority: Number,
	pp: Number,
	acc: Number,
	targeting: String,
	num_targets: Number,
	damage_info: { power: Number, crit_chance: Number },
	effects: [{
		effect: String, 
		status: String, 
		chance: Number,
		target: String, 
		stat: String,
		stages: Number,
		field_object: String,
		recoil_type: String,
		percent: Number,
		heal_percent: Number,
		type_removed: String,
		use_level: Boolean
	}], 
	contact: Boolean,
	protectable: Boolean,
	first_turn: Boolean
})

// export User model for use in other files.
module.exports = mongoose.model('move', MoveModel, 'moves');
