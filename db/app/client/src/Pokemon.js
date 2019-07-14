import React from 'react';
import { Container, Col, Button, Form, FormGroup, Label, Input, Card, CardBody, CardTitle } from 'reactstrap';


// When I show multiple pokemon at once, I may want 
// to just rename this class CurrentPokemon


class Pokemon extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			Species: '',
			Type0: '',
			Type1: '',
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

		fetch('./pokemonapi', {
			method: 'POST', 
			headers: {"Content-Type": "application/json"},
			body: JSON.stringify({ Species: this.state.Species, hardcoded: true })
		})
		.then(res => res.json())
		.then(res => console.log(res));


		// fetch('./api/pokemon', {
		// 	method: 'GET',
		// 	headers: {
		// 		'Content-Type': 'application/json'
		// 	},
		// 	body: this.state
		// })
		// .then(res => res.json())
  //   	.then(res => {
  //     		console.log("Connected to express:", res)
  //     	});
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

		        <Form>
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
