# galley-recipes
> this package replace internal recipe codes in [galley](https://github.com/rainist/galley) to reduce the complexity

## Install

`$ yarn add galley-recipes # or npm install galley-recipes`

## How to use

> Try [galley](https://rainist.github.io/galley) first to see what this package is for

### and put them in your code like below

```js
import { envMapper } from 'galley-recipes'
const { generate } = envMapper;

const output = generate({
  namespace: "my-namespace",
  cmName: "my-cm",
  cmEnv: "a=b\nc=d",
  secretName: "my-secret",
  secretEnv: "what=secret\nshouldbe=hidden"
});
```

```js
import { fileCM } from 'galley-recipes'
const { generate } = fileCM;

const output = generate({
  name: 'my-file-cm',
  namespace: 'my-namespace',
  filename: 'code.js',
  filecontent: 'console.log("Hellow, World!");',
});
```

## Test

`$ yarn test`

## Contribution
[any contributions are welcome.](https://github.com/rainist/galley-recipes/issues/new)

## License

galley-recipes is MIT licensed.
