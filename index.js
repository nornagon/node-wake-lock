const native = require('./build/Release/caffeine')

module.exports = {
  createWakeLock(reason, options) {
    options = {type: 'idle', ...(options || {})};
    return native.createPMAssertion(reason, options.type === 'display');
  },
  releaseWakeLock(lock) {
    return native.releasePMAssertion(lock);
  }
}
