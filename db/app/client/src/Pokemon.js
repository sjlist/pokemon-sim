import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card, CardBody, CardTitle } from 'reactstrap';


class PokemonCard extends React.Component {
	constructor(props) {
		super(props);

		this.onUpdate = this.onUpdate.bind(this);
		this.onDelete = this.onDelete.bind(this);
	}

	onUpdate() {
		return undefined;
	}

	onDelete() {
		return undefined;
	}

	render() {
		return(
			<Container>
				<Card>
					<Row>
						<Col sm={{ size: 3, offset: 1 }}>
							<p> {this.props.pokemon.Species} </p>
						</Col>
						<Col sm={{ size: 2, offset: 1 }}>
							<Button size="sm" color="primary" outline> Update </Button>
						</Col>
						<Col sm={{ size: 2, offset: 1 }}>
							<Button size="sm" color="danger" outline> Delete </Button>
						</Col>
					</Row>
				</Card>
			</Container>
		);
	}
}


class PokemonForm extends React.Component {
	render() {
		return(
			<Container>
				<Card>
					PokemonForm
				</Card>
			</Container>
		);
	}
}


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
		this.toggleAddStatus = this.toggleAddStatus.bind(this);
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

	toggleAddStatus() {
		this.setState({ addStatus: !this.state.addStatus });
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
		fetch('/api/pokemon', {
			method: 'GET'
		})
		.then(res => res.json())
		.then(res => this.setState({allpokemon: res}));
		console.log('got all pokemon');

	}

	deletePokemon(pokemon) {
		console.log('deleted pokemon');
		return undefined;
	}

	render() {
		return (
			<Container>
				<Row>
					<Col>
						<h3> Pokemon in DB </h3>
						{
							this.state.allpokemon.map(pkm => <PokemonCard {...this.state} pokemon={pkm} key={pkm._id} />)
						}
					</Col>
					<Col>
						<PokemonForm />
					</Col>
				</Row>
			</Container>
		);
	}
}

export default Pokemon;
