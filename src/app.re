module Button = {
  let component = ReasonReact.statelessComponent("Button");
  let make = (~display, ~action, _children) => {
    ...component,
    render: (_self) =>
      <button onClick=action style=(ReactDOMRe.Style.make(~width="60px", ()))>
        (ReasonReact.stringToElement(display))
      </button>
  };
};

module Display = {
  let component = ReasonReact.statelessComponent("Display");
  let make = (~value, _chidlren) => {
    ...component,
    render: (_self) => <p> (ReasonReact.stringToElement(value)) </p>
  };
};

type operation =
  | Divide
  | Multiply
  | Plus
  | Minus;

type action =
  | NumberPart(char)
  | Decimal
  | Operation(operation)
  | Equals
  | Clear
  | PlusMinus
  | Percentage;

type valueState = {
  currentValue: string,
  hasDecimal: bool,
  negative: bool
};

type state = {
  displayValue: string,
  currentOperator: option(operation),
  currentState: option(valueState),
  previousState: option(float)
};

let component = ReasonReact.reducerComponent("App");

let initialState = () => {
  currentState: None,
  displayValue: "0",
  currentOperator: None,
  previousState: None
};

let displayFloat = (float) => {
  let string = string_of_float(float);
  let length = String.length(string);
  if (string.[length - 1] == '.') {
    String.sub(string, 0, length - 1)
  } else {
    string
  }
};

let floatOfValueState = (valueState) =>
  switch valueState {
  | None => 0.0
  | Some({currentValue, negative}) => (negative ? (-1.0) : 1.0) *. float_of_string(currentValue)
  };

let stringOfValueState = (valueState) =>
  switch valueState {
  | None => "0"
  | Some({currentValue, negative}) => (negative ? "-" : "") ++ currentValue
  };

let calculateValue = (state) => {
  let currentFloat = floatOfValueState(state.currentState);
  let previousState =
    switch state.previousState {
    | None => currentFloat
    | Some(value) => value
    };
  switch state.currentOperator {
  | None => currentFloat
  | Some(operator) =>
    switch operator {
    | Divide => previousState /. currentFloat
    | Multiply => previousState *. currentFloat
    | Plus => previousState +. currentFloat
    | Minus => previousState -. currentFloat
    }
  }
};

let reducer = (action, state) =>
  switch action {
  | NumberPart(char) =>
    let currentState =
      switch (state.currentState, char) {
      | (None, '0') => None
      | (None, _) => Some({negative: false, currentValue: String.make(1, char), hasDecimal: false})
      | (Some({currentValue, hasDecimal, negative}), _) =>
        Some({negative, currentValue: currentValue ++ String.make(1, char), hasDecimal})
      };
    let displayValue = stringOfValueState(currentState);
    ReasonReact.Update({...state, currentState, displayValue})
  | Decimal =>
    let currentState =
      switch state.currentState {
      | None => Some({negative: false, currentValue: "0.", hasDecimal: true})
      | Some({currentValue, hasDecimal, negative}) as currentState =>
        hasDecimal ?
          currentState : Some({negative, currentValue: currentValue ++ ".", hasDecimal: true})
      };
    let displayValue = stringOfValueState(currentState);
    ReasonReact.Update({...state, currentState, displayValue})
  | Operation(operation) =>
    let currentOperator = Some(operation);
    let (displayValue, previousState) =
      switch state.currentState {
      | None => (state.displayValue, state.previousState)
      | Some(_) =>
        let value = calculateValue(state);
        (displayFloat(value), Some(value))
      };
    ReasonReact.Update({currentOperator, displayValue, currentState: None, previousState})
  | Equals =>
    switch state.currentState {
    | None => ReasonReact.NoUpdate
    | Some(_) =>
      let previousState = calculateValue(state);
      ReasonReact.Update({...initialState(), displayValue: displayFloat(previousState)})
    }
  | Percentage =>
    let previousState = calculateValue(state) /. 100.0;
    ReasonReact.Update({
      ...state,
      currentState: None,
      previousState: Some(previousState),
      displayValue: displayFloat(previousState)
    })
  | PlusMinus =>
    switch state.currentState {
    | None => ReasonReact.NoUpdate
    | Some(currentState) =>
      let currentState = Some({...currentState, negative: currentState.negative ? false : true});
      ReasonReact.Update({...state, currentState, displayValue: stringOfValueState(currentState)})
    }
  | Clear =>
    let newState =
      switch state.currentState {
      | Some(_) => {...state, currentState: None, displayValue: "0"}
      | None => initialState()
      };
    ReasonReact.Update(newState)
  };

let createButton = (reduce, display, action) => <Button action=(reduce((_) => action)) display />;

let make = (_children) => {
  ...component,
  reducer,
  initialState,
  render: ({reduce, state: {displayValue, currentState}}) => {
    let createButton = createButton(reduce);
    let clearDisplay =
      switch currentState {
      | None => "AC"
      | Some(_) => "C"
      };
    <div>
      <table>
        <tbody>
          <tr> <td colSpan=4> <Display value=displayValue /> </td> </tr>
          <tr>
            <td> (createButton(clearDisplay, Clear)) </td>
            <td> (createButton("+/-", PlusMinus)) </td>
            <td> (createButton("%", Percentage)) </td>
            <td> (createButton("/", Operation(Divide))) </td>
          </tr>
          <tr>
            <td> (createButton("7", NumberPart('7'))) </td>
            <td> (createButton("8", NumberPart('8'))) </td>
            <td> (createButton("9", NumberPart('9'))) </td>
            <td> (createButton("*", Operation(Multiply))) </td>
          </tr>
          <tr>
            <td> (createButton("4", NumberPart('4'))) </td>
            <td> (createButton("5", NumberPart('5'))) </td>
            <td> (createButton("6", NumberPart('6'))) </td>
            <td> (createButton("+", Operation(Plus))) </td>
          </tr>
          <tr>
            <td> (createButton("1", NumberPart('1'))) </td>
            <td> (createButton("2", NumberPart('2'))) </td>
            <td> (createButton("3", NumberPart('3'))) </td>
            <td> (createButton("-", Operation(Minus))) </td>
          </tr>
          <tr>
            <td colSpan=2> (createButton("0", NumberPart('0'))) </td>
            <td> (createButton(".", Decimal)) </td>
            <td> (createButton("=", Equals)) </td>
          </tr>
        </tbody>
      </table>
    </div>
  }
};