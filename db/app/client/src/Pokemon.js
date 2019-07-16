import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card, CardBody, CardTitle } from 'reactstrap';


class Pokemon extends React.Component {
	constructor(props) {
		super(props);

		// set state
		this.state = { addStatus: true, allpokemon: [], currpokemon: {} };

		// CRUD (create, return, update, delete) functionality
		this.createPokemon = this.createPokemon.bind(this);
		this.updatePokemon = this.updatePokemon.bind(this);
		this.getAllPokemon = this.getAllPokemon.bind(this);
		this.deletePokemon = this.deletePokemon.bind(this);

		// Helper functions
		this.handleCurrpokemonChange = this.handleCurrpokemonChange.bind(this);
	}

	componentDidMount() {
		console.log('Pokemon componentDidMount... fetching all pokemon in db');
		this.getAllPokemon();
	}

	handleCurrpokemonChange(event) {
		this.setState({
			currpokemon: { 
				...this.state.currpokemon, 
				[event.target.name]: event.target.value 
			}
		});
	}

	createPokemon(pokemon) {
		console.log('created pokemon');
		return undefined;
	}

	updatePokemon(pokemon) {
		console.log('updated pokemon');
		return undefined;
	}

	getAllPokemon() {
		console.log('got all pokemon');
		return undefined;
	}

	deletePokemon(pokemon) {
		console.log('deleted pokemon');
		return undefined;
	}

	render() {
		return (
			<Container>
				<p>
					PokemonAPI
				</p>
			</Container>
		);
	}
}

export default Pokemon;
