>>>+[>> while we are not at the end
  +[> read in first number
    ,>+++++[-<--------->]< read in digit and check if it's a minus sign
  ]
  <[[->+<]<]>>-<+>> set up conversion
  [---<[->++++++++++<]>>] convert to number
  <<-[+>[-<+>]<<-]>[-<+>] move into place
 
  >+[> read in second number
    ,>++++[-<-------->]< read in digit and check if it's a space
  ]
  <[[->+<]<]>>-<+>> set up conversion
  [----------------<[->++++++++++<]>>] convert to number
  <<-[+>[-<+>]<<-]>[-<+>] move into place
 
  >, read in character
 
  >>,,[-] ignore some characters
  >+[-<,+ read in character and check if it's not a newline or eof
    >>+<<[>]>>[ if it's eof
      <<<[<]<<<<<<<->>>>>>>>[>]>>-> stop execution and reading
    ]
    <<<-----------[>]>>[if it's a newline
      -> stop reading
    ]
    <<<++++++++++>> reset back to original
  ]
  <<[-]<[<]
  >[< while characters left
    <[-<+>>+<]<[->+<] copy check character
    >>[->-<] subtract from current character
    >[<]<<[ if match
      <<<+>> ] >>>[-]>[[-<+>]>]<<[<] shift
      >
    ] <<[-] cleanup
 
  <<[->>+<<]<[->+>+<<]>[-<+>]>>[-<<+>>]<< set up check
  >>>+<<<[->-[>]>>[<<<[-]>>>>]<<<<]>[<+>[-]]>>-<< check if x is bigger than max
  <[->>>>+<<<<]<<[->>+<<]> setup
  >>>+<<<[->-[>]>>[<<<[-]>>>>]<<<<]>[<+>[-]]>>-<< check if min is bigger than x
  >>>>[-<<<<<+>>>>>]<<<+<< or them together
  [>]>>[ if between bounds
    <<<<<<+>>>>>>> valid password
  ]
  <-<<[-]<<< cleanup
]
>+<<[>]>>[ if zero passwords are valid
    >>++++++[-<++++++++>]<.[-] just print 0
]<-<<
[ while there is a number left
  >++++++++++<
  [->-[>+>>]>[+[-<+>]>+>>]<<<<<] get digit
  >[-]>[-<<+>>]>[-<<+>>]<< setup for next iteration
]
<[ go through the digits
  >++++++[-<++++++++>]<.[-]< print digit
]
