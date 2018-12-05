import json

class Poke:
    def __init__(self, name, species):
        self.name = name
        self.species = species
        self.iv = None
        self.gender = None
        self.moves = None
        self.status = "none"
        self.nature = None
        self.item = None
        self.level = None
        self.HP = None
        self.ATK = None
        self.DEF = None
        self.SPA = None
        self.SPD = None
        self.SPE = None

    def load_species(self):
        jsonSPEC = json.load(open("./species/{}.json".format(self.species)))

    def save(self):
        jsonOBJ = {
            'name': self.name,
            'species': self.species,
            'iv': self.iv,
            'gender': self.gender,
            'moves': self.moves,
            'nature': self.nature,
            'item': self.item,
            'level': self.level
        }
        json.dump(jsonOBJ, open("./pokes/{}-{}.json".format(self.species, self.name), 'w'))

    def load(self):
        jsonOBJ = json.load(open("./pokes/{}-{}.json".format(self.species, self.name)))
        self.name = jsonOBJ['name']
        self.species = jsonOBJ['species']
        self.iv = jsonOBJ['iv']
        self.gender = jsonOBJ['gender']
        self.moves = jsonOBJ['moves']
        self.nature = jsonOBJ['nature']
        self.item = jsonOBJ['item']
        self.level = jsonOBJ['level']
