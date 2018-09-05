module Transformer = {
  let header = {|apiVersion: v1
kind: ConfigMap
data:
|};

  let filenameLine = (filename) =>
    "  " ++ filename ++ ": |\n";

  let filecontentBlock = (filecontent) =>
    filecontent
      |> Js.String.split("\n")
      |> Array.to_list
      |> List.map((line) => "    " ++ line)
      |> String.concat("\n")
      |> (joined) => joined ++ "\n";

  let metadata = (name, ns) =>
    "metadata: \n" ++
    "  name: " ++ name ++ "\n" ++
    "  namespace: " ++ ns ++ "\n";

  let _transform = (name, ns, filename, filecontent) =>
    header ++
    filenameLine(filename) ++
    filecontentBlock(filecontent) ++
    metadata(name, ns);

  let errMsg = (oname, ons, ofilename, ofilecontent) => {
    let missingKey = switch (oname, ons, ofilename, ofilecontent) {
    | (None, _, _, _) => "name"
    | (_, None, _, _) => "namespace"
    | (_, _, None, _) => "filename"
    | (_, _, _, None) => "filecontent"
    | _ => "mistery"
    };
    "[" ++ missingKey ++ "] is missing";
  };

  [@bs.deriving {jsConverter: newType}]
  type configMap = {
    name: option(string),
    namespace: option(string),
    filename: option(string),
    filecontent: option(string)
  };

  let transform = ({name: oname, namespace: ons, filename: ofilename, filecontent: ofilecontent}) =>
    switch (oname, ons, ofilename, ofilecontent) {
    | (Some(name), Some(ns), Some(filename), Some(filecontent)) => _transform(name, ns, filename, filecontent)
    | _ => errMsg(oname, ons, ofilename, ofilecontent)
    };

  let transformByObject = absCM =>
    absCM->configMapFromJs->transform;
};

let initialModel = [%bs.obj {
  placeholder: {
    namespace: "my-namespace",
    name: "new-cm",
    file: {
      name: "file.txt",
      content: "write something here"
    }
  }
}];

let default = [%bs.obj {
  generate: Transformer.transformByObject,
  initialModel: initialModel
}];
