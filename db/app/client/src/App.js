import React from 'react';
import { BrowserRouter, Switch, Route } from "react-router-dom";

import Menu from './Menu';
import Home from './Home';
import Pokemon from './Pokemon';
import Moves from './Moves';
import Teams from './Teams';
import NotFound from './NotFound';


class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {pkmn: ''};

    this.changePokemon = this.changePokemon.bind(this);
  }

  changePokemon(species) {
    this.setState({ pkmn: species });
  }

  componentDidUpdate (prevProps, prevState) {
    if (prevState.pkmn !== this.state.pkmn) {
      console.log('SUCCESSFULY UPDATED APP POKEMON');
    }
  }

  testAPI() {
    fetch('/api/testAPI', {
      method: 'GET'
    })
    .then(res => res.json())
    .then(res => console.log("api test:", res));
  }

  testDB() {
    fetch('/api/testDB', {
      method: 'GET'
    })
    .then(res => res.json())
    .then(res => console.log("DB connect test:", res));
  }
  
  componentDidMount () {
    console.log('In: App componentDidMount');
    this.testAPI();
    this.testDB();
  }

  render() {
    return (
      <BrowserRouter>
        <div className="App">
          <Menu />
          <Switch>
            <Route exact path='/' component={Home} />
            <Route exact path='/pokemon' component={Pokemon} />
            <Route exact path='/moves' component={Moves} />
            <Route exact path='/teams' component={Teams} />
            <Route component={NotFound} />
          </Switch>
        </div>
      </BrowserRouter>
    );
  }
}

export default App;
