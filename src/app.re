/*
 TODO Figure out partial application
 TODO Refactor pieces to their own files
 TODO Minimise repetition, understand scoping
 TODO Set up build so that it doens't fill src with crap
*/
type operation =
  | Divide
  | Multiply
  | Plus
  | Minus;

type action =
  | NumberPart(char)
  | Decimal
  | Operation(operation)
  | Equals;

type state = {
  displayValue: string,
  currentOperator: option(operation),
  currentValue: float
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  reducer: (action, state) => 
    switch action {
    | NumberPart(a) =>
      let {displayValue} = state;
      let newValue =
        if (displayValue == "0") {
          String.make(1, a)
        } else {
          displayValue ++ String.make(1, a)
        };
      ReasonReact.Update({...state, displayValue: newValue})
    | Operation(op) =>
      let {displayValue} = state;
      /* let add = (x, y) => x + y; 
      let partialOperation = add(int_of_string(displayValue)); */
      ReasonReact.Update({
        displayValue: "0",
        currentValue: float_of_string(displayValue),
        currentOperator: Some(op)
      });
    | Equals => 
      switch state.currentOperator {
      | Some(Plus) => 
        let {displayValue, currentValue} = state;
        let result = currentValue +. float_of_string(displayValue);
        ReasonReact.Update({
          displayValue: string_of_float(result),
          currentValue: float_of_string(displayValue),
          currentOperator: None
        }); 
      | _ => ReasonReact.NoUpdate
      };
      
    | _ => ReasonReact.NoUpdate
    },
  initialState: () => {displayValue: "0", currentOperator: None, currentValue: 0.0},
  render: ({state: {displayValue: value}, reduce}) =>
    <div>
      /* <Buttons /> */
      <Display value />
      <button onClick=(reduce((_) => NumberPart('9')))>
        (ReasonReact.stringToElement("9"))
      </button>
      <button onClick=(reduce((_) => NumberPart('8')))>
        (ReasonReact.stringToElement("8"))
      </button>
      <button onClick=(reduce((_) => Operation(Plus)))>
        (ReasonReact.stringToElement("+"))
      </button>
      <button onClick=(reduce((_) => Equals))>
        (ReasonReact.stringToElement("="))
      </button>
    </div>
};

    /*| Operation(op) =>
       ReasonReact.Update({
        ...state,
        currentValue: displayValue,
        currentValue: int_of_string(displayValue),
        currentOperator: Some(op),
        any: (a) => a
      }) 
      switch op {
      | plus => 
        let add = (x, y) => x + y;
        let partialOperation = add(int_of_string(displayValue));
        ReasonReact.Update({
          ...state,
          displayValue: displayValue ++ " +"
        });
      };*/