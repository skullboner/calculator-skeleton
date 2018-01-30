let component = ReasonReact.statelessComponent("Display");

let make = (~value, _chidlren) => {
    ...component,
    render: (_self) => <p> (ReasonReact.stringToElement(value)) </p>
};