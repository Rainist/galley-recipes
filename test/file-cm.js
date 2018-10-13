const assert = require('assert');

const { fileCM } = require('../dist/main');

describe('generate FileCM', () => {
  describe('generate', () => {
    const { generate } = fileCM;

    const input = {
      name: 'new-cm',
      namespace: 'my-namespace',
      filename: 'file.text',
      filecontent: 'write something here'
    };

    const output = `apiVersion: v1
kind: ConfigMap
data:
  file.text: |
    write something here
metadata:
  name: new-cm
  namespace: my-namespace
`;

    it('should be same with pregenerated output', () => {
      assert.equal(generate(input), output);
    });
  });
});
