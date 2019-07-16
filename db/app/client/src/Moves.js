import React from 'react';
import { Container, Col, Row, Form, FormGroup, Label, Input, Button, Card } from 'reactstrap';


class MoveCard extends React.Component {
	constructor(props) {
		super(props);

		this.state = { addStatus: this.props.addStatus, move: this.props.move };

		this.onUpdate = this.onUpdate.bind(this);
		this.onDelete = this.onDelete.bind(this);
	}

	componentDidUpdate(prevProps, prevState) {
		// Doesn't seem like it's needed, but I may have missed a test somewhere along the way where it helps
		if (prevProps.addStatus !== this.props.addStatus) { 
			this.setState({ addStatus: this.props.addStatus });
		}

		// Definately needed. Makes sure cards update with the move
		if (JSON.stringify(prevProps.move) !== JSON.stringify(this.props.move)) {
			this.setState({ move: this.props.move });
		}
	}

	onUpdate() {
		console.log('update started');
		this.props.setcurrMove(this.state.move);
		this.props.toggleAddStatus();
	}

	onDelete() {
		this.props.deleteMove(this.state.move);
	}

	render() {
		return(
			<Container>
				<Card>
					<Row>
						<Col sm={{ size: 3, offset: 1 }}>
							<p> {this.state.move.name} </p>
						</Col>
						<Col sm={{ size: 2, offset: 1 }}>
							{
								this.state.addStatus ?
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


// const MoveModel = new mongoose.Schema({
// 	name: String,
// 	damage_type: String,
// 	type: String, 
// 	priority: Number,
// 	pp: Number,
// 	acc: Number,
// 	targeting: String,
// 	num_targets: Number,
// 	damage_info: { power: Number, crit_change: Number },
// 	effects: [{effect: String, status: String, change: Number}], 
// 	contact: { type: Boolean }
// })


class MoveForm extends React.Component {
	constructor(props) {
		super(props);

		this.state = {name: '', damage_type: '', type: '', priority: 0, pp: 0, acc: 100, targeting: '', num_targets: 1, damage_info: { power: 0, crit_change: 0 }, effects: [], contact: true};

		this.handleFormChange = this.handleFormChange.bind(this);
		this.handleSubmit = this.handleSubmit.bind(this);
	}

	componentDidUpdate(prevProps, prevState) {
		if (prevProps.currMove._id !== this.props.currMove._id) { 
			this.setState(this.props.currMove);
		}
	}

	handleFormChange(event) {
		const name = event.target.name;

		if (name === 'power' || name === 'crit_change') {
			this.setState({damage_info: {...this.state.damage_info, [name]: event.target.value}});
		} else {
			this.setState({[name]: event.target.value});
		}
	}

	handleSubmit(event) {
		event.preventDefault();
		this.props.onSubmit(this.state);

		this.setState({name: '', damage_type: '', type: '', priority: 0, pp: 0, acc: 100, targeting: '', num_targets: 1, damage_info: { power: 0, crit_change: 0 }, effects: [], contact: false});

		if (!this.props.addStatus) {
			this.props.toggleAddStatus();
		}
	}

	render() {
		const types = ["NONE", "NORMAL", "FIRE", "WATER", "ELECTRIC", "GRASS", "ICE", "FIGHTING", "POISON", "GROUND", "FLYING", "PSYCHIC", "BUG", "ROCK", "GHOST", "DRAGON", "DARK", "STEEL", "FAIRY"];
		const typesList = types.map((t, i) => <option key={i}>{t}</option>);

		const effects = ["STAT_CHANGE", "VOLATILE_STATUS"];
		const effectsOptions = effects.map((eff, i) => <option key={i}>{eff}</option>);

		return(
			<Container>
				<h3> { this.props.addStatus ? 'Add Move' : 'Edit Move' } </h3> 
				<Card>
					<Form onSubmit={this.handleSubmit}>
						<FormGroup row>
							<Label for="name" sm={{ size: 3, offset: 1 }}> Name </Label>
							<Col sm={8}>
								<Input type="text" name="name" id="name" value={this.state.name} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="type" sm={{ size: 3, offset: 1 }}> Type </Label>
							<Col sm={8}>
							<Input type="select" name="type" id="type" value={this.state.type} onChange={this.handleFormChange}>
								{ typesList }
							</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="priority" sm={{ size: 3, offset: 1 }}> Priority </Label>
							<Col sm={8}>
								<Input type="number" name="priority" id="priority" value={this.state.priority} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="pp" sm={{ size: 3, offset: 1 }}> pp </Label>
							<Col sm={8}>
								<Input type="pp" name="pp" id="pp" value={this.state.pp} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="acc" sm={{ size: 3, offset: 1 }}> Acc. </Label>
							<Col sm={8}>
								<Input type="text" name="acc" id="acc" value={this.state.acc} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="targeting" sm={{ size: 3, offset: 1 }}> Targeting </Label>
							<Col sm={8}>
								<Input type="select" name="targeting" id="targeting" value={this.state.targeting} onChange={this.handleFormChange}>
									<option> ADJACENT_ALL 	</option>
									<option> ADJACENT_ENEMY </option>
									<option> SELF 			</option>
								</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="num_targets" sm={{ size: 3, offset: 1 }}> Num. Targets </Label>
							<Col sm={8}>
								<Input type="select" name="num_targets" id="num_targets" value={this.state.num_targets} onChange={this.handleFormChange}>
									<option> 0 </option>
									<option> 1 </option>
									<option> 2 </option>
									<option> 3 </option>
									<option> 4 </option>
								</Input>
							</Col>
						</FormGroup>

						Damage Info:
						<FormGroup row>
							<Label for="power" sm={{ size: 3, offset: 1 }}> Priority </Label>
							<Col sm={8}>
								<Input type="number" name="power" id="power" value={this.state.damage_info.power} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="crit_change" sm={{ size: 3, offset: 1 }}> Crit. Change </Label>
							<Col sm={8}>
								<Input type="number" name="crit_change" id="crit_change" value={this.state.damage_info.crit_change} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label check>
								<Input type="checkbox" name="contact" id="contact" value={this.state.contact} onChange={this.handleFormChange} /> Contact
							</Label>
						</FormGroup>

						{
							this.props.addStatus ?
								<Button color="primary" outline> Add Move </Button> :
								<Button color="primary" outline> Update Move </Button>
						}
					</Form>
				</Card>
			</Container>
		);
	}
}


class Moves extends React.Component {
	constructor(props) {
		super(props);

		// set state
		this.state = { addStatus: true, allMoves: [], currMove: {} };

		// CRUD (create, return, update, delete) functionality
		this.createMove = this.createMove.bind(this);
		this.updateMove = this.updateMove.bind(this);
		this.getAllMoves = this.getAllMoves.bind(this);
		this.deleteMove= this.deleteMove.bind(this);

		// Helper functions
		this.toggleAddStatus = this.toggleAddStatus.bind(this);
		this.setcurrMove = this.setcurrMove.bind(this);
	}

	componentDidMount() {
		console.log('Move componentDidMount... fetching all moves in db');
		this.getAllMoves();
	}

	toggleAddStatus() {
		this.setState({ addStatus: !this.state.addStatus });
	}

	setcurrMove(move) {
		this.setState({ currMove: move });
		console.log('set current move to ', move);
	}

	createMove(move) {
		fetch('/api/moves', {
			method: 'POST',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(move)
		})
		.then(res => res.json())
		.then(res => {
			console.log('created move:', res);
			this.setState({ allMoves: [ ...this.state.allMoves, res], currMove: {} });
		});
	}

	updateMove(move) {
		console.log('updating move');

		fetch('/api/moves', {
			method: 'PUT',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(move)
		})
		.then(res => res.json())
		.then(res => {
			console.log('updated move:', res);
			const moveIndex = this.state.allMoves.findIndex(mv => mv._id === res._id);
			let newallMoves = this.state.allMoves;
			newallMoves[moveIndex] = res;
			this.setState({ allMoves: newallMoves, currMove: {} });
		});
	}

	getAllMoves() {
		fetch('/api/moves', {
			method: 'GET'
		})
		.then(res => res.json())
		.then(res => this.setState({allMoves: res}));
	}

	deleteMove(move) {
		fetch('/api/moves', {
			method: 'DELETE',
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(move)
		})
		.then(res => res.json())
		.then(res => {
			console.log("deleted move from db:", res);
			const newallMoves = this.state.allMoves.filter(function(value, index, arr) {
				return value._id !== res._id;
			});
			this.setState({ allMoves: newallMoves });
		});
	}

	render() {
		return (
			<Container>
				<Row>
					<Col>
						<h3> Moves in DB </h3>
						{
							this.state.allMoves.map(mv => <MoveCard 
																{...this.state} 
																move={mv} 
																key={mv._id} 
																toggleAddStatus={this.toggleAddStatus} 
																setcurrMove={this.setcurrMove} 
																deleteMove={this.deleteMove} 
															/>)
						}
					</Col>
					<Col>
						{
							this.state.addStatus ? 
								<MoveForm 
									{...this.state} 
									setcurrMove={this.setcurrMove} 
									toggleAddStatus={this.toggleAddStatus} 
									onSubmit={this.createMove}
								/> :
								<MoveForm 
									{...this.state} 
									setcurrMove={this.setcurrMove} 
									toggleAddStatus={this.toggleAddStatus} 
									onSubmit={this.updateMove}
								/>
						}
					</Col>
				</Row>
			</Container>
		);
	}
}


export default Moves;
