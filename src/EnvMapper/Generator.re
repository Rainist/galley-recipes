type nameAndEnv = {
  name: string,
  env: string
};

type envMapperInput = {
  namespace: string,
  cm: nameAndEnv,
  secret: nameAndEnv
};

[@bs.deriving {jsConverter: newType}]
type envMapperOutput = {
  cm: string,
  secret: string,
  envSnippet: string
};

type base64; /* abstract type for a document object */
[@bs.send] external encodeBase64: (base64, string) => string = "encode";
[@bs.module "js-base64"] external base64: base64 = "Base64";

let encode = str => encodeBase64(base64, str);

let indent = (text, indenter) =>
  text
    |> Js.String.split("\n")
    |> Array.to_list
    |> List.map(line => indenter ++ line)
    |> String.concat("\n");

type its = envMapperInput => string;

let genCM: its = ({namespace, cm}) => {
  let {name, env}: nameAndEnv = cm;

  let yamlEnv = {
    let yaml = ((key, value)) => {
      Some({j|$(key): "$(value)"|j});
    };

    let unpackStr = (maybeStr) => switch (maybeStr) {
    | Some(str) => str
    | None => "something went wrong"
    };

    env
      |> Js.String.split("\n")
      |> Array.to_list
      |> List.map(line =>
        line
          ->StrUtils.keyValueOf
          ->Belt.Option.flatMap(yaml)
          ->unpackStr
        )
      |> String.concat("\n")
      |.indent("  ");
  };

  {j|apiVersion: v1
kind: ConfigMap
data:
$(yamlEnv)
metadata:
  name: $(name)
  namespace: $(namespace)|j};
};

let genSecret: its = ({namespace, secret}) => {
  let {name, env}: nameAndEnv = secret;

  let encEnv = "envENv";
  let encEnv = {
    let encrypt = ((key, value)) => {
      let encValue = value->encode;
      Some({j|$(key): $(encValue)|j});
    };

    let unpackStr = (maybeStr) => switch (maybeStr) {
    | Some(str) => str
    | None => "something went wrong"
    };

    env
      |> Js.String.split("\n")
      |> Array.to_list
      |> List.map(line =>
        line
          ->StrUtils.keyValueOf
          ->Belt.Option.flatMap(encrypt)
          ->unpackStr
        )
      |> String.concat("\n")
      |.indent("  ");
  };

  {j|apiVersion: v1
kind: Secret
type: Opaque
data:
$(encEnv)
metadata:
  name: $(name)
  namespace: $(namespace)|j};
};

let genSnippetPiece = (key, name, refType) => {
  {j|
- name: $(key)
  valueFrom:
    $(refType):
      key: $(key)
      name: $(name)|j};
};

let genSnippet: its = ({cm, secret}) => {
  let extractKey = maybeKV => switch(maybeKV) {
  | Some((key, _v)) => Some(key)
  | None => None
  };

  let createSnippet = (maybeKey, name, refType) => switch(maybeKey) {
  | Some(key) => genSnippetPiece(key, name, refType)
  | None => "no key found here"
  };

  let snippetsOf: nameAndEnv => string => string =
    ({name, env}, refType) => {

    env
      |> Js.String.split("\n")
      |> Array.to_list
      |> List.map(line =>
        line
          ->StrUtils.keyValueOf
          ->extractKey
          ->createSnippet(name, refType)
        )
      |> String.concat("")
  };

  "# Paste this into your deployment"
  ++
  snippetsOf(cm, "configMapKeyRef")
  ++
  snippetsOf(secret, "secretKeyRef")
  |. indent("        ")
  |. String.trim
};

type ito = envMapperInput => envMapperOutput;

let gen: ito = input => {
  cm: input->genCM,
  secret: input->genSecret,
  envSnippet: input->genSnippet,
};
