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
    "metadata:\n" ++
    "  name: " ++ name ++ "\n" ++
    "  namespace: " ++ ns ++ "\n";

  let _transform = (name, ns, filename, filecontent) =>
    header ++
    filenameLine(filename) ++
    filecontentBlock(filecontent) ++
    metadata(name, ns);

  let errMsg = (name, ns, filename, filecontent) => {
    let missingKey = switch (name, ns, filename, filecontent) {
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

  let transform = ({name, namespace, filename, filecontent}) =>
    switch (name, namespace, filename, filecontent) {
    | (Some(n), Some(ns), Some(fn), Some(fc)) => _transform(n, ns, fn, fc)
    | _ => errMsg(name, namespace, filename, filecontent)
    };

  let transformByObject = absCM =>
    absCM->configMapFromJs->transform;
};

let default = [%bs.obj {
  generate: Transformer.transformByObject
}];
