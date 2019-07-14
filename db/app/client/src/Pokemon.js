import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card, CardBody, CardTitle } from 'reactstrap';


// TODO:

// function passed into form to add current pokemon to all pokemon after submit
// another class for editing or deleting -> and corresponding CRUD functions
// function to select current pokemon from list to edit

// 		this.state = { allpokemon: [ { Species: '1', Type0: 'FIRE', Type1: 'NONE', HP: 0, ATK: 0, DEF: 0, SPA: 0, SPD: 0, SPE: 0},  { Species: '2', Type0: 'PHYCHIC', Type1: 'FLYING', HP: 0, ATK: 0, DEF: 0, SPA: 0, SPD: 0, SPE: 0}], CurrentPokemon: { Species: '', Type0: '', Type1: '', HP: 0, ATK: 0, DEF: 0, SPA: 0, SPD: 0, SPE: 0} };

class Pokemon extends React.Component {
	constructor(props) {
		super(props);
		this.state = { allpokemon: [], CurrentPokemon: { Species: '', Type0: '', Type1: '', HP: 0, ATK: 0, DEF: 0, SPA: 0, SPD: 0, SPE: 0} };

		this.addToPokemonList = this.addToPokemonList.bind(this);
	}

	componentDidMount() {
		console.log('Pokemon componentDidMount');

		fetch('/api/pokemon', {
			method: 'GET'
		})
		.then(res => res.json())
		.then(res => this.setState({allpokemon: res}));
	}

	addToPokemonList(newPokemon) {
		this.setState({ allpokemon: [ ...this.state.allpokemon, newPokemon] });
	}

	render() {
		return (
			<Row>
			<Col>
				{
					this.state.allpokemon.map((pkm, i) => <PokemonCard pokemon={pkm} key={i} />)
				}
			</Col>
			<Col>
				<PokemonForm {...this.state} addToPokemonList={this.addToPokemonList} />
			</Col>
			</Row>
		);
	}
}


class PokemonCard extends React.Component {
	render() {
		return(
			<Container>
				<Card>
					<CardTitle>
						<h3> { this.props.pokemon.Species } </h3>
					</CardTitle>
						<Button outline color="info" disabled> { this.props.pokemon.Type.type0 } </Button> 
						{' '}
						<Button outline color="info" disabled> { this.props.pokemon.Type.type1 } </Button> 
						<p>
						HP: {this.props.pokemon.HP}, ATK: {this.props.pokemon.ATK}, DEF: {this.props.pokemon.DEF}, SPA: {this.props.pokemon.SPA}, SPD: {this.props.pokemon.SPD}, SPE: {this.props.pokemon.SPE}
						</p>
				</Card>
			</Container>
		);
	}
}


class PokemonForm extends React.Component {
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
			<div className="Pokemon">
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
