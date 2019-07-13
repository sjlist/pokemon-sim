const mongoose = require('mongoose');

const Schema = mongoose.Schema;

const PokemonModel = new Schema({
	species: { type: String },
	type: { type0: String, type1: String }, 
	HP:  { type: Number },
	ATK: { type: Number },
	DEF: { type: Number },
	SPA: { type: Number },
	SPD: { type: Number },
	SPE: { type: Number }
})

// export User model for use in other files.
module.exports = PokemonModel;
