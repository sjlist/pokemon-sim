import React from 'react';
import { Jumbotron } from 'reactstrap';

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
          <ul>
            <li>
              Make add forms
            </li>
            <li>
              Allow for viewing existing entries
            </li>
            <li>
              Add editing and deleting existing entries
            </li>
          </ul>
        </Jumbotron>
      </div>
    );
  }
}

export default Home;
