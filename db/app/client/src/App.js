import React from 'react';
import { BrowserRouter, Switch, Route } from "react-router-dom";

import Menu from './Menu';
import Home from './Home';
import Pokemon from './Pokemon';
import Moves from './Moves';
import Teams from './Teams';
import NotFound from './NotFound';



function App() {
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

export default App;
