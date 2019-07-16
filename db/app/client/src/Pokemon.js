import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card } from 'reactstrap';


class PokemonCard extends React.Component {
	constructor(props) {
		super(props);

		// this.state = { addStatus: this.props.addStatus };

		this.onUpdate = this.onUpdate.bind(this);
		this.onDelete = this.onDelete.bind(this);
	}

	// componentDidUpdate(prevProps, prevState) {
	// 	if (prevProps.addStatus !== this.props.addStatus) { 
	// 		this.setState({ addStatus: this.props.addStatus });
	// 		console.log('change addStatus -> updated PokemonCard state');
	// 	}
	// }

	onUpdate() {
		console.log('update started');
		this.props.setCurrpokemon(this.props.pokemon);
		this.props.toggleAddStatus();
	}

	onDelete() {
		this.props.deletePokemon(this.props.pokemon);
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
							{
								this.props.addStatus ?
									<Button size="sm" color="primary" outline onClick={this.onUpdate}> Update </Button> :
									<Button size="sm" color="secondary" outline> Update </Button>
							}
						</Col>
						<Col sm={{ size: 2, offset: 1 }}>
							{
								this.props.addStatus ?
									<Button size="sm" color="danger" outline onClick={this.onDelete}>  Delete </Button> :
									<Button size="sm" color="secondary" outline> Delete </Button>
							}
						</Col>
					</Row>
				</Card>
			</Container>
		);
	}
}


class PokemonForm extends React.Component {
	constructor(props) {
		super(props);

		this.state = {Species: '', Type: {type0: "NONE", type1: "NONE"}, "HP": 0, "ATK": 0, "DEF": 0, "SPA": 0, "SPD": 0, "SPE": 0};

		this.handleFormChange = this.handleFormChange.bind(this);
		this.handleSubmit = this.handleSubmit.bind(this);
	}

	componentDidUpdate(prevProps, prevState) {
		// console.log('PokemonForm updated', this.state);
		if (prevProps.currpokemon._id !== this.props.currpokemon._id) { 
			this.setState(this.props.currpokemon);
			console.log('change in current pokemon -> updated PokemonForm state');
		}
	}

	handleFormChange(event) {
		const name = event.target.name;

		if (name === 'type0' || name === 'type1') {
			this.setState({Type: {...this.state.Type, [name]: event.target.value}});
		} else {
			this.setState({[name]: event.target.value});
		}
	}

	handleSubmit(event) {
		event.preventDefault();
		this.props.onSubmit(this.state);

		this.setState({Species: '', Type: {type0: "NONE", type1: "NONE"}, "HP": 0, "ATK": 0, "DEF": 0, "SPA": 0, "SPD": 0, "SPE": 0});

		if (!this.props.addStatus) {
			this.props.toggleAddStatus();
		}
	}

	render() {
		const types = ["NONE", "NORMAL", "FIRE", "WATER", "ELECTRIC", "GRASS", "ICE", "FIGHTING", "POISON", "GROUND", "FLYING", "PSYCHIC", "BUG", "ROCK", "GHOST", "DRAGON", "DARK", "STEEL", "FAIRY"];
		const typesList = types.map((t, i) => <option key={i}>{t}</option>);

		return(
			<Container>
				<h3> { this.props.addStatus ? 'Add Pokemon' : 'Edit Pokemon' } </h3> 
				<Card>
					<Form onSubmit={this.handleSubmit}>
						<FormGroup row>
							<Label for="species" sm={{ size: 3, offset: 1 }}> Species </Label>
							<Col sm={8}>
								<Input type="text" name="Species" id="species" value={this.state.Species} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="type0" sm={{ size: 3, offset: 1 }}> Type 0 </Label>
							<Col sm={8}>
							<Input type="select" name="type0" id="type0" value={this.state.Type.type0} onChange={this.handleFormChange}>
								{ typesList }
							</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="type1" sm={{ size: 3, offset: 1 }}> Type 1 </Label>
							<Col sm={8}>
							<Input type="select" name="type1" id="type1" value={this.state.Type.type1} onChange={this.handleFormChange}>
								{ typesList }
							</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="HP" sm={{ size: 3, offset: 1 }}> HP </Label>
							<Col sm={8}>
								<Input type="number" name="HP" id="HP" value={this.state.HP} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="ATK" sm={{ size: 3, offset: 1 }}> ATK </Label>
							<Col sm={8}>
								<Input type="number" name="ATK" id="ATK" value={this.state.ATK} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="DEF" sm={{ size: 3, offset: 1 }}> DEF </Label>
							<Col sm={8}>
								<Input type="number" name="DEF" id="DEF" value={this.state.DEF} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="SPA" sm={{ size: 3, offset: 1 }}> SPA </Label>
							<Col sm={8}>
								<Input type="number" name="SPA" id="SPA" value={this.state.SPA} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="SPD" sm={{ size: 3, offset: 1 }}> SPD </Label>
							<Col sm={8}>
								<Input type="number" name="SPD" id="SPD" value={this.state.SPD} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="SPE" sm={{ size: 3, offset: 1 }}> SPE </Label>
							<Col sm={8}>
								<Input type="number" name="SPE" id="SPE" value={this.state.SPE} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						{
							this.props.addStatus ?
								<Button color="primary" outline> Add Pokemon </Button> :
								<Button color="primary" outline> Update Pokemon </Button>
						}
					</Form>
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
		this.toggleAddStatus = this.toggleAddStatus.bind(this);
		this.setCurrpokemon = this.setCurrpokemon.bind(this);
	}

	componentDidMount() {
		console.log('Pokemon componentDidMount... fetching all pokemon in db');
		this.getAllPokemon();
	}

	toggleAddStatus() {
		this.setState({ addStatus: !this.state.addStatus });
	}

	// Similar to handleCurrpokemonChange but this function takes a pokemon not a 
	// change in text field. I could use this function and a local current pokemon 
	// in the form rather than a global current pokemon and an event changer.
	setCurrpokemon(pokemon) {
		this.setState({ currpokemon: pokemon });
		console.log('set current pokemon to ', pokemon);
	}

	createPokemon(pokemon) {
		fetch('/api/pokemon', {
			method: 'POST',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(pokemon)
		})
		.then(res => res.json())
		.then(res => {
			console.log('created pokemon:', res);
			this.setState({ allpokemon: [ ...this.state.allpokemon, res], currpokemon: {} });
		});
	}

	updatePokemon(pokemon) {
		console.log('updating pokemon');

		fetch('/api/pokemon', {
			method: 'PUT',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(pokemon)
		})
		.then(res => res.json())
		.then(res => {
			console.log('updated pokemon:', res);
			const pokemonIndex = this.state.allpokemon.findIndex(pkm => pkm._id === res._id);
			let newallpokemon = this.state.allpokemon;
			newallpokemon[pokemonIndex] = res;
			this.setState({ allpokemon: newallpokemon });
		});
	}

	getAllPokemon() {
		fetch('/api/pokemon', {
			method: 'GET'
		})
		.then(res => res.json())
		.then(res => this.setState({allpokemon: res}));
	}

	deletePokemon(pokemon) {
		fetch('/api/pokemon', {
			method: 'DELETE',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(pokemon)
		})
		.then(res => res.json())
		.then(res => {
			console.log("deleted pokemon from db:", res);
			const newallpokemon = this.state.allpokemon.filter(function(value, index, arr) {
				return value._id !== res._id;
			});
			this.setState({ allpokemon: newallpokemon });
		});
	}

	render() {
		return (
			<Container>
				<Row>
					<Col>
						<h3> Pokemon in DB </h3>
						{
							this.state.allpokemon.map(pkm => <PokemonCard 
																{...this.state} 
																pokemon={pkm} 
																key={pkm._id} 
																toggleAddStatus={this.toggleAddStatus} 
																setCurrpokemon={this.setCurrpokemon} 
																deletePokemon={this.deletePokemon} 
															/>)
						}
					</Col>
					<Col>
						{
							this.state.addStatus ? 
								<PokemonForm 
									{...this.state} 
									setCurrpokemon={this.setCurrpokemon} 
									toggleAddStatus={this.toggleAddStatus} 
									onSubmit={this.createPokemon}
								/> :
								<PokemonForm 
									{...this.state} 
									setCurrpokemon={this.setCurrpokemon} 
									toggleAddStatus={this.toggleAddStatus} 
									onSubmit={this.updatePokemon}
								/>
						}
					</Col>
				</Row>
			</Container>
		);
	}
}

export default Pokemon;
