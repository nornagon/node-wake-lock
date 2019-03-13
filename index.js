const native = require('./build/Release/caffeine');

class WakeLock {
  constructor(reason, options) {
    options = {type: 'idle', ...(options || {})};
    this._lock = native.createPMAssertion(reason, options.type === 'display');
  }
  release() {
    if (!this._lock) {
      throw new Error("Lock already released")
    }
    native.releasePMAssertion(this._lock);
    this._lock = null;
  }
}

module.exports = WakeLock;
