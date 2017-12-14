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
    | _ => ReasonReact.NoUpdate
    },
  initialState: () => {displayValue: "0", currentOperator: None, currentValue: 0.0},
  render: ({state: {displayValue: value}, reduce}) =>
    <div>
      <Display value />
      <button onClick=(reduce((_) => NumberPart('9')))>
        (ReasonReact.stringToElement("9"))
      </button>
      <button onClick=(reduce((_) => NumberPart('8')))>
        (ReasonReact.stringToElement("8"))
      </button>
    </div>
};