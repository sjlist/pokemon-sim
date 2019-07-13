import React from 'react';
import { Navbar, NavbarBrand, NavbarToggler, Collapse, Nav, NavItem, NavLink } from 'reactstrap';



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
          <NavbarBrand href="/">Pokemon Battle Sim</NavbarBrand>
          <NavbarToggler onClick={this.toggle} />
          <Collapse isOpen={this.state.isOpen} navbar>

            <Nav className="ml-auto" navbar>
              <NavItem>
                <NavLink href="/pokemon">Add Pokemon</NavLink>
              </NavItem>
              <NavItem>
                <NavLink href="/moves">Add Moves</NavLink>
              </NavItem>
              <NavItem>
                <NavLink href="/teams">Build Teams</NavLink>
              </NavItem>
            </Nav>
          </Collapse>
        </Navbar>
      </div>
    );
  }
}

export default Menu;