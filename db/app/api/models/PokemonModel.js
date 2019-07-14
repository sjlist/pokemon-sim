const mongoose = require('mongoose');

const PokemonModel = new mongoose.Schema({
	Species: { type: String },
	Type: { type0: String, type1: String }, 
	HP:  { type: Number },
	ATK: { type: Number },
	DEF: { type: Number },
	SPA: { type: Number },
	SPD: { type: Number },
	SPE: { type: Number }
})

// export User model for use in other files.
module.exports = mongoose.model('pokemon', PokemonModel, 'pokemon');
