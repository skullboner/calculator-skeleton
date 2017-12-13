type operation =
  | Divide
  | Multiply
  | Plus
  | Minus;

type action =
  | NumberPart(char)
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
  reducer: (action: action, _state: state) =>
    switch action {
    | _ => ReasonReact.NoUpdate
    },
  initialState: () => {displayValue: "", currentOperator: None, currentValue: 0.0},
  render: (_self) => <div> (ReasonReact.stringToElement("This is a calculator!")) </div>
};