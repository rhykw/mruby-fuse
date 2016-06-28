MRuby::Build.new do |conf|
  toolchain :gcc

  conf.gembox 'default'
  conf.gem '../../mruby-fuse' do |g|
    g.cc.flags << '-g -D_FILE_OFFSET_BITS=64'
    g.linker.flags << '-lfuse'
  end
  conf.enable_test
end
