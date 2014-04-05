camera [
  eye 20 15 -15
  look 0 0 -1
  focus 5 3 -30
  up 0 1 0
  angle 60
  near-far 0.1 100
]

light [
  type point
  position 10 10 10
  direction -10 -10 -10
  color 1 1 1
  function 1 0 0
]

background [
   color 0 0 0.5
]

mastersubgraph wallpiece [
	trans [
		scale 4 4 2.5
		object cube [
			diffuse 1 0.9 0.8
			specular 0 0 1
		]
	]
]

mastersubgraph walls [
	trans [
		translate 4 2 0 
		scale 12 8 2.5
		object cube [
			diffuse 1 0.9 0.8
			specular 0 0 1
		]
	]
]
mastersubgraph deco [
	trans [
		scale 1.5 1.5 3 
		object cube [
			diffuse 0.8 0.6 0.2
			specular 0 0 0.5
			shine 0.8
		]
	]
]

mastersubgraph frontwall [
	trans [
		subgraph wallpiece 
	]	
	trans [
		translate 8 0 0
		subgraph wallpiece 
	]
	trans [
		translate 4 4 0
		scale 11 4 2.5
		object cube [
			diffuse 1 0.9 0.8
			specular 0 0 1
		]
	]
	trans [
		translate 1 4 0
		subgraph deco
	]
	trans [
		translate 7 4 0
		subgraph deco
	]
]

mastersubgraph castlewalls [
	trans [
		subgraph frontwall 
	]
	trans [
		translate 0 0 -12
		subgraph walls
	]
	trans [
		translate -2 0 -2
		rotate 0 1 0 90
		subgraph walls
	]
	trans [
		translate 10 0 -2
		rotate 0 1 0 90
		subgraph walls
	]
]

mastersubgraph columns [
	trans [
		translate -2 2.5 0
		scale 4 9 4
		object cylinder [
			diffuse 1 0.9 0.8
			specular 0 0 1
		]
	]
]

mastersubgraph roofs [
	trans [
		translate -2 2.5 0
		scale 4.5 5 4.5
		object cone [
			diffuse 1 0.2 0.2
			specular 0 0 1
			reflect 1 1 0
		]
	]
]

mastersubgraph rooftops [
	trans [
		translate -2 3.2 0
		object sphere [
			transparent 1 1 0
			alpha 0.9
			specular .1 1 0
			shine 0.5
		]
	]
]

mastersubgraph towers [
	trans [
		translate 0 6.5 0
		subgraph roofs
	]
	trans [
		subgraph columns
	]
]

mastersubgraph castle [
	trans [
		subgraph castlewalls
	]
	trans [
		subgraph towers
	]
	trans [
		translate 12 0 0
		subgraph towers
	]
	trans [
		translate 0 0 -12
		subgraph towers
	]
	trans [
		translate 12 0 -12
		subgraph towers
	]
	trans [
		translate 0 8.5 0
		subgraph rooftops
	]
	trans [
		translate 12 8.5 0
		subgraph rooftops
	]
	trans [
		translate 4 -3 -7
		scale 40 0.2 50
		object cube [
			diffuse 0.1 1 0.2
			specular 0.2 0.9 0.1
		]
	]
]

mastersubgraph root [
  trans [
    translate 0 0 -30
    subgraph castle
  ]
]