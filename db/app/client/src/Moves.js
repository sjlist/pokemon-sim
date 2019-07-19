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
						<Col size="5">
							<p> {this.state.move.name} </p>
						</Col>
						<Col>
							{
								this.state.addStatus ?
									<Button size="sm" color="primary" outline onClick={this.onUpdate}> Update </Button> :
									<Button size="sm" color="secondary" outline> Update </Button>
							}
						</Col>
						<Col>
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


class MoveForm extends React.Component {
	constructor(props) {
		super(props);

		this.blankMove = {name: '', damage_type: 'physical', type: 'NONE', priority: 0, pp: 0, acc: 100, targeting: 'ADJACENT_ALL', num_targets: 1, damage_info: { power: 0, crit_chance: 0 }, effects: [], contact: true, protectable: true, first_turn: false};
		this.state = {...this.blankMove};

		this.handleFormChange = this.handleFormChange.bind(this);
		this.handleEffectFormChange = this.handleEffectFormChange.bind(this);
		this.handleSubmit = this.handleSubmit.bind(this);
		this.addEffect = this.addEffect.bind(this);
		this.deleteEffect = this.deleteEffect.bind(this);

		this.effectFields = this.effectFields.bind(this);
		this.effectCard = this.effectCard.bind(this);
	}

	componentDidUpdate(prevProps, prevState) {
		if (prevProps.currMove._id !== this.props.currMove._id) { 
			this.setState(this.props.currMove);
		}
	}

	handleFormChange(event) {
		const name = event.target.name;

		if (name === 'power' || name === 'crit_chance') {
			this.setState({damage_info: {...this.state.damage_info, [name]: event.target.value}});
		} else {
			this.setState({[name]: event.target.value});
		}
	}

	handleEffectFormChange(event, index) {
		const newEffects = this.state.effects;

		if (event.target.name === 'effect') {
			const id = newEffects[index]._id;
			newEffects.splice(index, 1, { _id: id, [event.target.name]: event.target.value }); // starting at index replace one element with new element
		}
		else {
			newEffects[index] = {...newEffects[index], [event.target.name]: event.target.value};
		}

		this.setState({ effects: newEffects });
	}

	handleSubmit(event) {
		event.preventDefault();
		this.props.onSubmit(this.state);

		this.setState({name: '', damage_type: 'physical', type: 'NONE', priority: 0, pp: 0, acc: 100, targeting: 'ADJACENT_ALL', num_targets: 1, damage_info: { power: 0, crit_chance: 0 }, effects: [], contact: true, protectable: true, first_turn: false});

		if (!this.props.addStatus) {
			this.props.toggleAddStatus();
		}
	}

	addEffect() {
		let newEffects = this.state.effects;
		newEffects.push({ });
		this.setState({ effects: newEffects });
	}

	deleteEffect(delete_index) {
		const newEffects = this.state.effects.filter(function(value, index, arr) {
			return index !== delete_index;
		})
		this.setState({ effects: newEffects });
	}

	effectFields(index) {
		const status_list = ['', 'BURNED', 'FROZEN', 'PARALYZED', 'POISONED', 'ASLEEP'];
		const status_options = status_list.map((status, i) => <option key={i}>{status}</option>);

		const stats_list = ['', 'HP', 'ATK', 'DEF', 'SPA', 'SPD', 'SPE'];
		const stats_options = stats_list.map((stat, i) => <option key={i}>{stat}</option>);

		const volatile_stats_list = ['', 'FLINCHED', 'CONFUSION', 'TAUNT'];
		const volatile_stats_options = volatile_stats_list.map((vstat, i) => <option key={i}>{vstat}</option>);

		const field_object_list = ['', 'LEECH_SEED', 'SPIKES', 'STEALTH_ROCKS', 'STICKY_WEB', 'TOXIC_SPIKES'];
		const field_object_options = field_object_list.map((fieldobj, i) => <option key={i}>{fieldobj}</option>);

		const types = ['','NORMAL', 'FIRE', 'WATER', 'ELECTRIC', 'GRASS', 'ICE', 'FIGHTING', 'POISON', 'GROUND', 'FLYING', 'PSYCHIC', 'BUG', 'ROCK', 'GHOST', 'DRAGON', 'DARK', 'STEEL', 'FAIRY'];
		const types_options = types.map((t, i) => <option key={i}>{t}</option>);

		switch(this.state.effects[index].effect) {
			case 'SWAP':
				return(
					<div>
						<FormGroup row>
							<Label for="target" sm={{ size: 3, offset: 1 }}> Target: </Label>
							<Col sm={8}>
								<Input type="select" name="target" id="target" value={this.state.effects[index].target} onChange={(event) => this.handleEffectFormChange(event, index)}>
									<option> </option>
									<option> SELF </option>
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			case 'STATUS':
				return(
					<div>
						<FormGroup row>
							<Label for="status" sm={{ size: 3, offset: 1 }}> Status: </Label>
							<Col sm={8}>
								<Input type="select" name="status" id="status" value={this.state.effects[index].status} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ status_options }
								</Input>
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="chance" sm={{ size: 3, offset: 1 }}> Chance: </Label>
							<Col sm={8}>
								<Input type="number" name="chance" id="chance" value={this.state.effects[index].chance || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="target" sm={{ size: 3, offset: 1 }}> Target: </Label>
							<Col sm={8}>
								<Input type="select" name="target" id="target" value={this.state.effects[index].target} onChange={(event) => this.handleEffectFormChange(event, index)}>
									<option> </option>
									<option> SELF </option>
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
				case 'VOLATILE_STATUS':
				return(
					<div>
						<FormGroup row>
							<Label for="status" sm={{ size: 3, offset: 1 }}> Status: </Label>
							<Col sm={8}>
								<Input type="select" name="status" id="status" value={this.state.effects[index].status} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ volatile_stats_options }
								</Input>
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="chance" sm={{ size: 3, offset: 1 }}> Chance: </Label>
							<Col sm={8}>
								<Input type="number" name="chance" id="chance" value={this.state.effects[index].chance  || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
					</div>
				);
			case 'STAT_CHANGE':
				return(
					<div>
						<FormGroup row>
							<Label for="stat" sm={{ size: 3, offset: 1 }}> Stat: </Label>
							<Col sm={8}>
								<Input type="select" name="stat" id="stat" value={this.state.effects[index].stat} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ stats_options }
								</Input>
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="stages" sm={{ size: 3, offset: 1 }}> Stages: </Label>
							<Col sm={8}>
								<Input type="number" name="stages" id="stages" value={this.state.effects[index].stages || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="chance" sm={{ size: 3, offset: 1 }}> Chance: </Label>
							<Col sm={8}>
								<Input type="number" name="chance" id="chance" value={this.state.effects[index].chance || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="target" sm={{ size: 3, offset: 1 }}> Target: </Label>
							<Col sm={8}>
								<Input type="select" name="target" id="target" value={this.state.effects[index].target} onChange={(event) => this.handleEffectFormChange(event, index)}>
									<option> </option>
									<option> SELF </option>
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			case 'FIELD_CHANGE':
				return(
					<div>
						<FormGroup row>
							<Label for="field_object" sm={{ size: 3, offset: 1 }}> Field Object: </Label>
							<Col sm={8}>
								<Input type="select" name="field_object" id="field_object" value={this.state.effects[index].field_object} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ field_object_options }
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			case 'RECOIL':
				return(
					<div>
						<FormGroup row>
							<Label for="recoil_type" sm={{ size: 3, offset: 1 }}> Recoil Type: </Label>
							<Col sm={8}>
								<Input type="select" name="recoil_type" id="recoil_type" value={this.state.effects[index].recoil_type} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ stats_options }
								</Input>
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="percent" sm={{ size: 3, offset: 1 }}> Percent: </Label>
							<Col sm={8}>
								<Input type="number" name="percent" id="percent" value={this.state.effects[index].percent || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
					</div>
				);
			case 'HEAL':
				return(
					<div>
						<FormGroup row>
							<Label for="heal_percent" sm={{ size: 3, offset: 1 }}> Heal Percent: </Label>
							<Col sm={8}>
								<Input type="number" name="heal_percent" id="heal_percent" value={this.state.effects[index].heal_percent || ''} onChange={(event) => this.handleEffectFormChange(event, index)} />
							</Col>
						</FormGroup>
						<FormGroup row>
							<Label for="target" sm={{ size: 3, offset: 1 }}> Target: </Label>
							<Col sm={8}>
								<Input type="select" name="target" id="target" value={this.state.effects[index].target} onChange={(event) => this.handleEffectFormChange(event, index)}>
									<option> </option>
									<option> SELF </option>
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			case 'REMOVE_TYPE':
				return(
					<div>
						<FormGroup row>
							<Label for="type_removed" sm={{ size: 3, offset: 1 }}> Type Removed: </Label>
							<Col sm={8}>
								<Input type="select" name="type_removed" id="type_removed" value={this.state.effects[index].type_removed} onChange={(event) => this.handleEffectFormChange(event, index)}>
									{ types_options }
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			case 'FLAT_DAMAGE':
				return(
					<div>
						<FormGroup row>
							<Label for="use_level" sm={{ size: 3, offset: 1 }}> Use_Level: </Label>
							<Col sm={8}>
								<Input type="select" name="use_level" id="use_level" value={this.state.effects[index].use_level} onChange={(event) => this.handleEffectFormChange(event, index)}>
									<option> </option>
									<option> true  </option>
									<option> false </option>
								</Input>
							</Col>
						</FormGroup>
					</div>
				);
			default:
				return null;
		}
	}

	effectCard(index) {
		const effect_types = ['', 'SWAP', 'STATUS', 'VOLATILE_STATUS', 'STAT_CHANGE', 'FIELD_CHANGE', 'RECOIL', 'HEAL', 'REMOVE_TYPE', 'FLAT_DAMAGE', 'PROTECT', 'SUBSTITUTE'];
		const effects_type_Options = effect_types.map((effect, i) => <option key={i}>{effect}</option>);

		return(
			<Container key={index}>
				<Card>
					<Row> 
						<Col sm={9}>
							Effect { index }
						</Col>
						<Col sm={3}>
							<Button size="sm" color="danger" outline style={{float:'right'}} onClick={() => this.deleteEffect(index)}> Delete Effect </Button>
						</Col>
					</Row>
					<FormGroup row>
						<Label for="effect" sm={{ size: 3, offset: 1 }}> Effect: </Label>
						<Col sm={8}>
						<Input type="select" name="effect" id="effect" value={this.state.effects[index].effect} onChange={(event) => this.handleEffectFormChange(event, index)}>
							{ effects_type_Options }
						</Input>
						</Col>
					</FormGroup>

					<div>
					{
						this.effectFields(index)
					}
					</div>
				</Card>
			</Container>
		);
	}

	render() {
		const types = ['', 'NORMAL', 'FIRE', 'WATER', 'ELECTRIC', 'GRASS', 'ICE', 'FIGHTING', 'POISON', 'GROUND', 'FLYING', 'PSYCHIC', 'BUG', 'ROCK', 'GHOST', 'DRAGON', 'DARK', 'STEEL', 'FAIRY'];
		const typesList = types.map((t, i) => <option key={i}>{t}</option>);

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
							<Label for="damage_type" sm={{ size: 3, offset: 1 }}> Damge Type </Label>
							<Col sm={8}>
								<Input type="select" name="damage_type" id="damage_type" value={this.state.damage_type} onChange={this.handleFormChange}>
									<option> physical 	</option>
									<option> special 	</option>
									<option> status 	</option>
								</Input>
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
									<option> ALL_OTHERS		</option>
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
							<Label for="power" sm={{ size: 3, offset: 1 }}> Power </Label>
							<Col sm={8}>
								<Input type="number" name="power" id="power" value={this.state.damage_info.power} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="crit_chance" sm={{ size: 3, offset: 1 }}> Crit. Change </Label>
							<Col sm={8}>
								<Input type="number" name="crit_chance" id="crit_chance" value={this.state.damage_info.crit_chance} onChange={this.handleFormChange} />
							</Col>
						</FormGroup>

						{
							this.state.effects.map((effect, i) => this.effectCard(i))
						}
						<Row>
							<Col>
							<Button color="secondary" size="sm" outline onClick={this.addEffect}> Add Effect </Button>
							</Col>
						</Row>

						<FormGroup row>
							<Label for="contact" sm={{ size: 3, offset: 1 }}> Contact </Label>
							<Col sm={8}>
								<Input type="select" name="contact" id="contact" value={this.state.contact} onChange={this.handleFormChange}>
									<option> true  </option>
									<option> false </option>
								</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="protectable" sm={{ size: 4, offset: 0 }}> Protectable </Label>
							<Col sm={8}>
								<Input type="select" name="protectable" id="protectable" value={this.state.protectable} onChange={this.handleFormChange}>
									<option> true  </option>
									<option> false </option>
								</Input>
							</Col>
						</FormGroup>

						<FormGroup row>
							<Label for="first_turn" sm={{ size: 4, offset: 0 }}> First Turn: </Label>
							<Col sm={8}>
								<Input type="select" name="first_turn" id="first_turn" value={this.state.first_turn} onChange={this.handleFormChange}>
									<option> false  </option>
									<option> true </option>
								</Input>
							</Col>
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
