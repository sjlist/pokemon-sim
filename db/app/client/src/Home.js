import React from 'react';
import { Jumbotron, Input } from 'reactstrap';

class Home extends React.Component {
  render() {
    return (
      <div className="Home">
        <Jumbotron>
          <h1> 
            Hey There! 
          </h1>
          <p> 
            Welcome to the Pokemon Battle Sim webclient. This is a cobbled together api for the database used in the simulator. Here you can add pokemon, add moves, and build teams to use in the battler. Using the nav bar at the top you can access three forms to add these to the database. 
          </p>
          <h3>
            To-do:
          </h3>
          <div>
            <div>
              <Input type="checkbox" id="todo1" checked={false} /> Make add forms
            </div>
            <div>
              <Input type="checkbox" id="todo1" checked={false} /> Allow for viewing existing entries
            </div>
            <div>
              <Input type="checkbox" id="todo1" checked={false} /> Add editing and deleting existing entries
            </div>
          </div>
        </Jumbotron>
      </div>
    );
  }
}

export default Home;
