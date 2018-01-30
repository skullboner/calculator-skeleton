let component = ReasonReact.statelessComponent("Buttons");

let make = (~onClick, _chidlren) => {
    ...component,
    render: (_self) => {
        let buttons = ["0","1","2","3","4","5","6","7","8","9","+","-","/","*"];
        <div>(
            ReasonReact.arrayToElement(
                Array.of_list(
                    List.map( (button) => <button>(ReasonReact.stringToElement(button))</button>, buttons)
                )
            )
        )</div>
    } 
};

