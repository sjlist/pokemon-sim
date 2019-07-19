const mongoose = require('mongoose');

const PokemonModel = new mongoose.Schema({
	Species: String,
	Type: { type0: String, type1: String }, 
	HP:  Number,
	ATK: Number,
	DEF: Number,
	SPA: Number,
	SPD: Number,
	SPE: Number 
})

// export User model for use in other files.
module.exports = mongoose.model('pokemon', PokemonModel, 'pokemon');
