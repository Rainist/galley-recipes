const assert = require('assert');
const _ = require('lodash');

const { envMapper } = require('../dist/main');

describe('generate EnvMapper', () => {
  describe('generate', () => {
    const { generate } = envMapper;

    const input = {
      namespace: "my-namespace",
      cmName: "new-cm",
      cmEnv: `
      key=value

      key2=value2
      `,
      secretName: "new-secret",
      secretEnv: `
      key=value
      key2=value2
      `
    };

    const output = {
      cm: `apiVersion: v1
kind: ConfigMap
data:
  key: "value"
  key2: "value2"
metadata:
  name: new-cm
  namespace: my-namespace`,
      secret: `apiVersion: v1
kind: Secret
type: Opaque
data:
  key: dmFsdWU=
  key2: dmFsdWUy
metadata:
  name: new-secret
  namespace: my-namespace`,
      envSnippet: `# Paste this into your deployment
        - name: key
          valueFrom:
            configMapKeyRef:
              key: key
              name: new-cm
        - name: key2
          valueFrom:
            configMapKeyRef:
              key: key2
              name: new-cm
        - name: key
          valueFrom:
            secretKeyRef:
              key: key
              name: new-secret
        - name: key2
          valueFrom:
            secretKeyRef:
              key: key2
              name: new-secret`,
      errMsg: undefined
    };

    it('should be same with pregenerated output', () => {
      assert(_.isEqual(generate(input), output));
    });
  });
});
