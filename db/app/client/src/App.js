import React from 'react';
import { Navbar, NavbarBrand, NavbarToggler, Collapse, Nav, NavItem, NavLink } from 'reactstrap';
import { Jumbotron, Input } from 'reactstrap';

class Menu extends React.Component {
  constructor(props) {
    super(props);
    this.state = { isOpen: false };

    this.toggleOpen = this.toggleOpen.bind(this);
  }

  toggleOpen() {
    this.setState({ isOpen: !this.state.isOpen });
  }

  render() {
    return (
      <div className="Menu">
        <Navbar color="light" light expand="md">
          <NavbarBrand href="/">Pokemon Sim</NavbarBrand>
          <NavbarToggler onClick={this.toggle} />
          <Collapse isOpen={this.state.isOpen} navbar>
            <Nav className="ml-auto" navbar>
              <NavItem>
                <NavLink href="/pokemon/">Add Pokemon</NavLink>
              </NavItem>
              <NavItem>
                <NavLink href="/moves/">Add Moves</NavLink>
              </NavItem>
              <NavItem>
                <NavLink href="/teams/">Build Teams</NavLink>
              </NavItem>
            </Nav>
          </Collapse>
        </Navbar>
      </div>
    );
  }
}




function App() {
  return (
    <div className="App">
      <header className="App-header">
        <Menu />
        <Jumbotron>
          <h1> 
            Hey there! 
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
      </header>
    </div>
  );
}

export default App;
