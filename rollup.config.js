import bucklescript from "rollup-plugin-bucklescript";

export default {
  input: "src/main.re",
  output: {
    file: "dist/main.js",
    format: "cjs",
  },
  plugins: [bucklescript()],
  external: [ 'js-base64' ]
};
