import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card, CardBody, CardTitle } from 'reactstrap';


// TODO:

// another class for editing -> and corresponding CRUD function
// function to select current pokemon from list to edit


class Pokemon extends React.Component {
	constructor(props) {
		super(props);
		this.state = { add: true, allpokemon: [], CurrentPokemon: {} };

		this.setAddStatus = this.setAddStatus.bind(this);
		this.addToPokemonList = this.addToPokemonList.bind(this);
		this.setCurrPokemon = this.setCurrPokemon.bind(this);
		this.deletePokemon = this.deletePokemon.bind(this);
	}

	componentDidMount() {
		console.log('Pokemon componentDidMount');

		fetch('/api/pokemon', {
			method: 'GET'
		})
		.then(res => res.json())
		.then(res => this.setState({allpokemon: res}));
	}

	setAddStatus(status) {
		console.log("old status:", this.state.add, "new status:", status);
		this.setState({ add: status });
	}

	addToPokemonList(newPokemon) {
		this.setState({ allpokemon: [ ...this.state.allpokemon, newPokemon] });
	}

	setCurrPokemon(pokemon) {
		this.setState({ CurrentPokemon: pokemon });
	}

	deletePokemon(pokemon) {
		// console.log('deleting pokemon:', pokemon);
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
			<Row>
				<Col>
					{
						this.state.allpokemon.map((pkm, i) => <PokemonCard pokemon={pkm} key={i} setAddStatus={this.setAddStatus} setCurrPokemon={this.setCurrPokemon} deletePokemon={this.deletePokemon} />)
					}
				</Col>
				<Col>
					{
						this.state.add ? <PokemonAddForm {...this.state} addToPokemonList={this.addToPokemonList} /> : <PokemonEditForm {...this.state} setAddStatus={this.setAddStatus} setCurrPokemon={this.setCurrPokemon} handleChange={this.handleChange} /> 
					}
				</Col>
			</Row>
		);
	}
}


class PokemonCard extends React.Component {
	constructor(props) {
		super(props);

		this.selectPokemonEdit = this.selectPokemonEdit.bind(this);
	}

	selectPokemonEdit() {
		this.props.setAddStatus(false);
		this.props.setCurrPokemon(this.props.pokemon);
	}

	render() {
		return(
			<Container>
				<Card>
					<CardTitle>
						<h3> { this.props.pokemon.Species } </h3>
					</CardTitle>
					<Button onClick={this.selectPokemonEdit}> Edit </Button>
					<Button onClick={() => this.props.deletePokemon(this.props.pokemon)}> Delete </Button>
					{ /*
						<Button outline color="info" disabled> { this.props.pokemon.Type.type0 } </Button> 
						{' '}
						<Button outline color="info" disabled> { this.props.pokemon.Type.type1 } </Button> 
						<p>
						HP: {this.props.pokemon.HP}, ATK: {this.props.pokemon.ATK}, DEF: {this.props.pokemon.DEF}, SPA: {this.props.pokemon.SPA}, SPD: {this.props.pokemon.SPD}, SPE: {this.props.pokemon.SPE}
						</p>
					*/ }
				</Card>
			</Container>
		);
	}
}



class PokemonEditForm extends React.Component {
	constructor(props) {
		super(props);
		this.state = this.props.CurrentPokemon;

		this.updatePokemon = this.updatePokemon.bind(this);
		this.handleChange = this.handleChange.bind(this);
	}

	updatePokemon() {
		console.log('updating:', this.state);
		fetch('/api/pokemon', {
			method: 'PUT',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(this.state)
		})
		.then(res => res.json())
		.then(res => {
			console.log('put returned:', res);
			const pokemonIndex = this.props.allpokemon.findIndex(pkm => pkm._id === res._id);
			console.log('update at index:', pokemonIndex);
		});

		this.props.setAddStatus(true);
		this.props.setCurrPokemon({});
	}

	handleChange(event) {
		this.setState({ [event.target.name]: event.target.value });
	}

	render() {
		const types = ["NONE", "NORMAL", "FIRE", "WATER", "ELECTRIC", "GRASS", "ICE", "FIGHTING", "POISON", "GROUND", "FLYING", "PSYCHIC", "BUG", "ROCK", "GHOST", "DRAGON", "DARK", "STEEL", "FAIRY"];
		const typesList = types.map((t, i) => <option key={i}>{t}</option>);

		return (
			<div className="PokemonEditForm">
				<Container>

				<h1> Pokemon </h1>
					<Card>
						<CardBody>
							<CardTitle>
								<h3> Edit </h3>
							</CardTitle>

							<Form onSubmit={this.updatePokemon}>
								<FormGroup row>
									<Label for="species" sm={2}> Species </Label>
									<Col sm={10}>
										<Input type="text" name="Species" id="species" value={this.state.Species} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="Type0" sm={2}> Type 0 </Label>
									<Col sm={10}>
									<Input type="select" name="Type.type0" id="Type.type0" placeholder="NONE" value={this.state.Type.type0} onChange={this.handleChange}>
										{ typesList }
									</Input>
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="Type1" sm={2}> Type 1 </Label>
									<Col sm={10}>
									<Input type="select" name="Type.type1" id="Type.type1" placeholder="NONE" value={this.state.Type.type1} onChange={this.handleChange}>
										{ typesList }
									</Input>
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="HP" sm={2}> HP </Label>
									<Col sm={10}>
										<Input type="number" name="HP" id="HP" placeholder="0" value={this.state.HP} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="ATK" sm={2}> ATK </Label>
									<Col sm={10}>
										<Input type="number" name="ATK" id="ATK" placeholder="0" value={this.state.ATK} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="DEF" sm={2}> DEF </Label>
									<Col sm={10}>
										<Input type="number" name="DEF" id="DEF" placeholder="0" value={this.state.DEF} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPA" sm={2}> SPA </Label>
									<Col sm={10}>
										<Input type="number" name="SPA" id="SPA" placeholder="0" value={this.state.SPA} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPD" sm={2}> SPD </Label>
									<Col sm={10}>
										<Input type="number" name="SPD" id="SPD" placeholder="0" value={this.state.SPD} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPE" sm={2}> SPE </Label>
									<Col sm={10}>
										<Input type="number" name="SPE" id="SPE" placeholder="0" value={this.state.SPE} onChange={this.handleChange} />
									</Col>
								</FormGroup>
								<Button>Submit</Button>

							</Form>
						</CardBody>
					</Card>

				</Container>
			</div>
		);
	}
}


class PokemonAddForm extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			Species: '',
			Type0: 'NONE',
			Type1: 'NONE',
			HP: 0,
			ATK: 0,
			DEF: 0,
			SPA: 0,
			SPD: 0,
			SPE: 0
		};

		this.handleChange = this.handleChange.bind(this);
		this.handleSubmit = this.handleSubmit.bind(this);
	}

	handleChange(event) {
		this.setState({ [event.target.name]: event.target.value });
		// console.log(this.state);
	}

	handleSubmit(event) {
		event.preventDefault();

		fetch('/api/pokemon', {
			method: 'POST',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(this.state)
		})
		.then(res => res.json())
		.then(res => console.log("posted pokemon:", res));

		alert('Added pokemon ' + this.state.Species + ' to database');

		this.props.addToPokemonList({Species: this.state.Species, Type: {type0: this.state.Type0, type1: this.state.Type1}, HP: this.state.HP, ATK: this.state.ATK, DEF: this.state.DEF, SPA: this.state.SPA, SPD: this.state.SPD, SPE: this.state.SPE });
		this.setState({ Species: '', Type0: '', Type1: '', HP: 0, ATK: 0, DEF: 0, SPA: 0, SPD: 0, SPE: 0 });
	}

	render() {
		const types = ["NONE", "NORMAL", "FIRE", "WATER", "ELECTRIC", "GRASS", "ICE", "FIGHTING", "POISON", "GROUND", "FLYING", "PSYCHIC", "BUG", "ROCK", "GHOST", "DRAGON", "DARK", "STEEL", "FAIRY"];
		const typesList = types.map((t, i) => <option key={i}>{t}</option>);

		return (
			<div className="PokemonAddForm">
				<Container>

				<h1> Pokemon </h1>
					<Card>
						<CardBody>
							<CardTitle>
								<h3>Add a Pokemon</h3>
							</CardTitle>

							<Form onSubmit={this.handleSubmit}>
								<FormGroup row>
									<Label for="species" sm={2}> Species </Label>
									<Col sm={10}>
										<Input type="text" name="Species" id="species" value={this.state.Species} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="Type0" sm={2}> Type 0 </Label>
									<Col sm={10}>
									<Input type="select" name="Type0" id="Type0" placeholder="NONE" value={this.state.Type0} onChange={this.handleChange}>
										{ typesList }
									</Input>
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="Type1" sm={2}> Type 1 </Label>
									<Col sm={10}>
									<Input type="select" name="Type1" id="Type1" placeholder="NONE" value={this.state.Type1} onChange={this.handleChange}>
										{ typesList }
									</Input>
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="HP" sm={2}> HP </Label>
									<Col sm={10}>
										<Input type="number" name="HP" id="HP" placeholder="0" value={this.state.HP} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="ATK" sm={2}> ATK </Label>
									<Col sm={10}>
										<Input type="number" name="ATK" id="ATK" placeholder="0" value={this.state.ATK} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="DEF" sm={2}> DEF </Label>
									<Col sm={10}>
										<Input type="number" name="DEF" id="DEF" placeholder="0" value={this.state.DEF} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPA" sm={2}> SPA </Label>
									<Col sm={10}>
										<Input type="number" name="SPA" id="SPA" placeholder="0" value={this.state.SPA} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPD" sm={2}> SPD </Label>
									<Col sm={10}>
										<Input type="number" name="SPD" id="SPD" placeholder="0" value={this.state.SPD} onChange={this.handleChange} />
									</Col>
								</FormGroup>

								<FormGroup row>
									<Label for="SPE" sm={2}> SPE </Label>
									<Col sm={10}>
										<Input type="number" name="SPE" id="SPE" placeholder="0" value={this.state.SPE} onChange={this.handleChange} />
									</Col>
								</FormGroup>
								<Button>Submit</Button>

							</Form>
						</CardBody>
					</Card>

				</Container>
			</div>
		);
 	}
}


export default Pokemon;
