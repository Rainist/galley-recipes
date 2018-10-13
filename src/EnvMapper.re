
[@bs.deriving {jsConverter: newType}]
type jsEnvMapperInput = {
  namespace: option(string),
  cmName: option(string),
  cmEnv: option(string),
  secretName: option(string),
  secretEnv: option(string),
};

[@bs.deriving {jsConverter: newType}]
type jsEnvMapperOutput = {
  cm: option(string),
  secret: option(string),
  envSnippet: option(string),
  errMsg: option(string)
};

let toObject: Generator.envMapperOutput => jsEnvMapperOutput = ({cm, secret, envSnippet}) => {
  cm: Some(cm),
  secret: Some(secret),
  envSnippet: Some(envSnippet),
  errMsg: None
};

let _transform = (namespace, cmName, cmEnv, secretName, secretEnv) =>
  Generator.gen({
    namespace,
    cm: {
      name: cmName,
      env: cmEnv,
    },
    secret: {
      name: secretName,
      env: secretEnv
    }
  });

let errMsg = (namespace, cmName, cmEnv, secretName, secretEnv) => {
  let missingKey = switch (namespace, cmName, cmEnv, secretName, secretEnv) {
  | (None, _, _, _, _) => "namespace"
  | (_, None, _, _, _) => "cmName"
  | (_, _, None, _, _) => "cmEnv"
  | (_, _, _, None, _) => "secretName"
  | (_, _, _, _, None) => "secretEnv"
  | _ => "mistery"
  };

  let errMsg = "[" ++ missingKey ++ "] is missing";

  let result: jsEnvMapperOutput = {
    cm: None,
    secret: None,
    envSnippet: None,
    errMsg: Some(errMsg)
  };
  result;
};

let transform = ({namespace, cmName, cmEnv, secretName, secretEnv}) =>
  switch (namespace, cmName, cmEnv, secretName, secretEnv) {
  | (Some(ns), Some(cn), Some(ce), Some(sn), Some(se)) =>
    _transform(ns, cn, ce, sn, se)
    ->toObject
  | _ => errMsg(namespace, cmName, cmEnv, secretName, secretEnv)
  } |> jsEnvMapperOutputToJs;

let transformByObject = absInput =>
  absInput->jsEnvMapperInputFromJs->transform;

let default = [%bs.obj {
  generate: transformByObject
}];
