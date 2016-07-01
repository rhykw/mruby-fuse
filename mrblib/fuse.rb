class Fuse

  def initialize(path,opt="")
    @mountpoint = path
    @opt = opt
    @entries = {
      "/" => { "stat" => { "st_mode" => 0040755, "st_nlink" => 2 , "st_size" => 0, "st_mtime" => 1467223189 } }
    }
  end

  def mountpoint
    @mountpoint
  end

  def entries
    @entries
  end

  def entries=(val)
    @entries = val
  end

end
